#include "GameScene.h"
#include "GameActor.h"
#include <GameTFunc.h>
#include "GameTransform.h"
#include "GameRenderMgr.h"

std::map<GameString, SPtr<GameScene>> GameScene::m_AllMap;


SPtr<GameScene> GameScene::m_CurScene = nullptr;
SPtr<GameScene> GameScene::m_NextScene = nullptr;

//RENDERMGR�� �����ڴ� SCENE�� �ް� �Ǿ�����
GameScene::GameScene() : m_IsLoading(false), RENDERMGR(this)
{

}

GameScene::~GameScene()
{
}

//����� ���� �����Ҵ� �ؼ� �̸��� �ٿ��ְ�(GameName) ���� ����ƽ��� �ʿ� �־��ش�.
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
	//���� ����� �ִ� ���
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

		//���� ����� �ִٸ� ChangeEnd�� �������ְ� �ٲ���.
		m_NextScene->ChangeStart(m_CurScene);
		m_CurScene = m_NextScene;
	}

	//���� component�� update�� ���⼭ �����ش�.
	m_CurScene->UpdatePrev();
	m_CurScene->Update();
	m_CurScene->UpdateNext();
	m_CurScene->Collision();
	m_CurScene->Render();
	m_CurScene->Release();
}


void GameScene::Loading()
{
	//���� ������ �ִ� COM���� ��� Loading ���ش�.
	for (auto& Item : m_AllSceneCom)
	{
		Item->Loading();
	}

	m_IsLoading = true;
}

void GameScene::ChangeEnd(GameScene* _pNextScene)
{
	//SceneCom �����ֱ�
	for (auto& _SceneCom : m_AllSceneCom)
	{
		_SceneCom->ChangeEnd(_pNextScene);
	}
}

void GameScene::ChangeStart(GameScene* _pNextScene)
{
	//SceneCom �����ֱ�
	for (auto& _SceneCom : m_AllSceneCom)
	{
		_SceneCom->ChangeStart(_pNextScene);
	}
}




/////////GameComponent ������ ����� Scene�� CreateActor�� ȣ�����ش�.
//Scene()->CreateActor()�� �����Ƽ� �߰���.

SPtr<GameActor> GameScene::SceneComponent::CreateActor(const GameString& _Name /*= L""*/)
{
	return m_pScene->CreateActor(_Name);
}

SPtr<GameActor> GameScene::SceneComponent::FindActor(const GameString& _Name)
{
	return m_pScene->FindActor(_Name);
}

//���� CreateActor���� ȣ��� �Լ�
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

	//SceneCom �����ֱ�
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

	//SceneCom �����ֱ�
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

	//SceneCom �����ֱ�
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
	//���� �Ŵ������� �����ַ�
	RENDERMGR.Release();
	//�ݸŴ��� Release����


	m_StartIter = m_AllActor.begin();
	m_EndIter = m_AllActor.end();


	//�������� for���� ���� ���������� ������ �� ����.
	for (; m_StartIter != m_EndIter;)
	{
		//���Ͱ� �׾����� �����ַ�
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


	//�������� for���� ���� ���������� ������ �� ����.
	for (; m_AllSceneComStartIter != m_AllSceneComEndIter;)
	{
		//���Ͱ� �׾����� �����ַ�
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