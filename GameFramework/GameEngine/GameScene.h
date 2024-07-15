#pragma once
#include <GameString.h>
#include <GameRef.h>
#include <GameName.h>
#include <map>
#include <list>
#include "GameRenderMgr.h"
#include "GameColMgr.h"
#include "GameObjRef.h"

//게임의 장면 게임의 한 단락 SPINE
class GameActor;
class GameScene final : public GameObjRef, public GameName
{
private:
	friend GameRenderMgr;
public:
	class SceneComponent : public GameObjRef
	{
	public:
		//자신의 부모 장면을 가리키는 포인터
		GameScene* m_pScene;
		friend GameScene;

	public:
		virtual void Loading() {}
		//장면이 끝날때 실행해줄 함수
		virtual void ChangeEnd(GameScene* _pNextScene)	{} 
		//장면이 바뀌면서 이전 장면에서 뭔가를 해주고 싶다면 아래 함수를 사용한다.
		//스타듀벨리에서 장면이 바껴도 작물이 자라는 거 같은 내용.
		virtual void ChangeStart(GameScene* _pPrevScene) {} //장면이 시작될때 실행해줄 함수 
		virtual void Update() {}
		virtual void UpdateNext() {}
		virtual void UpdatePrev() {}
		virtual void DebugRender() {}

		GameScene* Scene()
		{
			return m_pScene;
		}

		SPtr<GameActor> CreateActor(const GameString& _Name = L"");
		SPtr<GameActor> FindActor(const GameString& _Name);
	};

	///////////Actor 관리/////////////
public:
	std::list<SPtr<GameActor>> m_AllActor;
	std::map<GameString, SPtr<GameActor>> m_FindActor;
	std::list<SPtr<GameActor>>::iterator m_StartIter;
	std::list<SPtr<GameActor>>::iterator m_EndIter;

	//////////멤버변수///////////////////
private:
	static std::map<GameString, SPtr<GameScene>> m_AllMap;

	static SPtr<GameScene> m_CurScene;
	static SPtr<GameScene> m_NextScene;

	bool m_IsLoading;
	//장면의 컴포넌트로 사용
	std::list<SPtr<SceneComponent>> m_AllSceneCom;
	std::list<SPtr<SceneComponent>>::iterator m_AllSceneComStartIter;
	std::list<SPtr<SceneComponent>>::iterator m_AllSceneComEndIter;

public:
	//랜더링용 멤버
	GameRenderMgr RENDERMGR;
	//콜리전용 멤버
	GameColMgr COLMGR;

public:
	GameScene();
	~GameScene();

public:
	static SPtr<GameScene> FindScene(const GameString& _Name);
	static SPtr<GameScene> CreateScene(const GameString& _Name);

	//CreateScene<컴포넌트자료형> 만들때 컴포넌트도 자동으로 넣어줌
	template<typename T, typename ... Rest>
	static SPtr<GameScene> CreateScene(const GameString& _Name)
	{
		SPtr<GameScene> NewScene = CreateScene(_Name);
		NewScene->AddCom<T>();
		return NewScene;
	}

	static void ChangeScene(const GameString& _Name);

	//장면안의 각종 update를 실행시켜줄 함수
	static void Progress();

	template<typename T>
	void AddCom()
	{
		T* NewCom = new T();
		NewCom->m_pScene = this;
		m_AllSceneCom.push_back(NewCom);
	}

	void Loading();

	static GameScene* CurScene()
	{
		return m_CurScene;
	}

	//////Actor 관리 함수
	//////Component의 CreateActor는 이걸 호출한다.
	//기본은 이름이 있으면 그걸로 찾을 수 있고 아니면 Update만 돈다.
	SPtr<GameActor> CreateActor(const GameString& _Name = L"");
	SPtr<GameActor> FindActor(const GameString& _Name);

	//Update 함수
	void UpdatePrev();
	void Update();
	void UpdateNext();
	//랜더링 함수, RenderMGR의 Render()를 호출해준다.
	void Render();
	void Collision();
	void DebugRender();
	void Release();

private:
	//장면에서 이전 장면을 넘어가며 뭔가 하고 싶을때
	void ChangeEnd(GameScene* _pNextScene);
	void ChangeStart(GameScene* _pPrevScene);
};

