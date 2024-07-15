#include "GameScene.h"
#include "GameActor.h"
#include <GameTFunc.h>
#include "GameTransform.h"
#include "GameRenderMgr.h"

std::map<GameString, SPtr<GameScene>> GameScene::m_AllMap;


SPtr<GameScene> GameScene::m_CurScene = nullptr;
SPtr<GameScene> GameScene::m_NextScene = nullptr;

//RENDERMGR의 생성자는 SCENE을 받게 되어있음
GameScene::GameScene() : m_IsLoading(false), RENDERMGR(this)
{

}

GameScene::~GameScene()
{
}

//장면을 새로 동적할당 해서 이름을 붙여주고(GameName) 내부 스태틱멤버 맵에 넣어준다.
SPtr<GameScene> GameScene::CreateScene(const GameString& _Name)
{
	GameScene* NewScene = new GameScene();
	NewScene->Name(_Name);
	m_AllMap.insert(std::map<GameString, SPtr<GameScene>>::value_type(NewScene->Name(), NewScene));

	return NewScene;
}

SPtr<GameScene> GameScene::FindScene(const GameString& _Name)
{
	return GameTFunc::MapFindPointer(m_AllMap, _Name);
}

void GameScene::ChangeScene(const GameString& _Name)
{
	m_NextScene = FindScene(_Name);
}

void GameScene::Progress()
{
	//다음 장면이 있는 경우
	if (nullptr != m_NextScene)
	{
		if (false == m_NextScene->m_IsLoading)
		{
			m_NextScene->Loading();
		}

		if (nullptr != m_CurScene)
		{
			m_CurScene->ChangeEnd(m_NextScene);
		}

		//다음 장면이 있다면 ChangeEnd를 실행해주고 바뀌자.
		m_NextScene->ChangeStart(m_CurScene);
		m_CurScene = m_NextScene;
	}

	//내부 component의 update를 여기서 돌려준다.
	m_CurScene->UpdatePrev();
	m_CurScene->Update();
	m_CurScene->UpdateNext();
	m_CurScene->Collision();
	m_CurScene->Render();
	m_CurScene->Release();
}


void GameScene::Loading()
{
	//내가 가지고 있는 COM들을 모두 Loading 해준다.
	for (auto& Item : m_AllSceneCom)
	{
		Item->Loading();
	}

	m_IsLoading = true;
}

void GameScene::ChangeEnd(GameScene* _pNextScene)
{
	//SceneCom 돌려주기
	for (auto& _SceneCom : m_AllSceneCom)
	{
		_SceneCom->ChangeEnd(_pNextScene);
	}
}

void GameScene::ChangeStart(GameScene* _pNextScene)
{
	//SceneCom 돌려주기
	for (auto& _SceneCom : m_AllSceneCom)
	{
		_SceneCom->ChangeStart(_pNextScene);
	}
}




/////////GameComponent 내부의 멤버로 Scene의 CreateActor를 호출해준다.
//Scene()->CreateActor()가 귀찮아서 추가됨.

SPtr<GameActor> GameScene::SceneComponent::CreateActor(const GameString& _Name /*= L""*/)
{
	return m_pScene->CreateActor(_Name);
}

SPtr<GameActor> GameScene::SceneComponent::FindActor(const GameString& _Name)
{
	return m_pScene->FindActor(_Name);
}

//위의 CreateActor에서 호출될 함수
SPtr<GameActor> GameScene::CreateActor(const GameString& _Name)
{
	GameActor * NewActor = new GameActor();
	NewActor->m_pScene = this;
	NewActor->m_pTrans = NewActor->AddCom<GameTransform>();

	if (_Name != L"")
	{
		NewActor->m_IsFind = true;
		NewActor->Name(_Name);
		m_FindActor.insert(std::map<GameString, SPtr<GameActor>>::value_type(_Name, NewActor));
	}

	m_AllActor.push_back(NewActor);

	return NewActor;
}

SPtr<GameActor> GameScene::FindActor(const GameString& _Name /*= L""*/)
{
	return GameTFunc::MapFindPointer(m_FindActor, _Name);
}


/////////UpdateFunc//////////////
void GameScene::UpdatePrev()
{
	for (auto& Actor : m_AllActor)
	{
		if (false == Actor->IsOn())
		{
			continue;
		}
		Actor->UpdatePrev();
	}

	//SceneCom 돌려주기
	for (auto& _SceneCom : m_AllSceneCom)
	{
		if (false == _SceneCom->IsOn())
		{
			continue;
		}
		_SceneCom->UpdatePrev();
	}
}

void GameScene::Update()
{
	for (auto& Actor : m_AllActor)
	{
		if (false == Actor->IsOn())
		{
			continue;
		}
		Actor->Update();
	}

	//SceneCom 돌려주기
	for (auto& _SceneCom : m_AllSceneCom)
	{
		if (false == _SceneCom->IsOn())
		{
			continue;
		}
		_SceneCom->Update();
	}
}

void GameScene::UpdateNext()
{
	for (auto& Actor : m_AllActor)
	{
		if (false == Actor->IsOn())
		{
			continue;
		}
		Actor->UpdateNext();
	}

	//SceneCom 돌려주기
	for (auto& _SceneCom : m_AllSceneCom)
	{
		if (false == _SceneCom->IsOn())
		{
			continue;
		}
		_SceneCom->UpdateNext();
	}
}

void GameScene::Render()
{
	RENDERMGR.Render();
}

void GameScene::Collision()
{
	COLMGR.Collision();
}

void GameScene::Release()
{
	COLMGR.Release();
	//랜더 매니저에서 지워주렴
	RENDERMGR.Release();
	//콜매니저 Release실행


	m_StartIter = m_AllActor.begin();
	m_EndIter = m_AllActor.end();


	//레인지드 for문은 내부 삭제연산을 진행할 수 없다.
	for (; m_StartIter != m_EndIter;)
	{
		//액터가 죽었으면 지워주렴
		if (false == (*m_StartIter)->IsDeath())
		{
			++m_StartIter;
			continue;
		}

		(*m_StartIter)->DeleteCom();
		m_StartIter = m_AllActor.erase(m_StartIter);
	}

	m_AllSceneComStartIter = m_AllSceneCom.begin();
	m_AllSceneComEndIter = m_AllSceneCom.end();


	//레인지드 for문은 내부 삭제연산을 진행할 수 없다.
	for (; m_AllSceneComStartIter != m_AllSceneComEndIter;)
	{
		//액터가 죽었으면 지워주렴
		if (false == (*m_AllSceneComStartIter)->IsDeath())
		{
			++m_AllSceneComStartIter;
			continue;
		}

		m_AllSceneComStartIter = m_AllSceneCom.erase(m_AllSceneComStartIter);
	}
}

void GameScene::DebugRender()
{
	for (auto& SceneCom : m_AllSceneCom)
	{
		if (false == SceneCom->IsUpdate())
		{
			continue;
		}

		SceneCom->DebugRender();
	}

	for (auto& Actor : m_AllActor)
	{
		if (false == Actor->IsUpdate())
		{
			continue;
		}

		Actor->DebugRender();
	}
}