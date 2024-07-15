#pragma once
#include <GameString.h>
#include <GameRef.h>
#include <GameName.h>
#include <map>
#include <list>
#include "GameRenderMgr.h"
#include "GameColMgr.h"
#include "GameObjRef.h"

//������ ��� ������ �� �ܶ� SPINE
class GameActor;
class GameScene final : public GameObjRef, public GameName
{
private:
	friend GameRenderMgr;
public:
	class SceneComponent : public GameObjRef
	{
	public:
		//�ڽ��� �θ� ����� ����Ű�� ������
		GameScene* m_pScene;
		friend GameScene;

	public:
		virtual void Loading() {}
		//����� ������ �������� �Լ�
		virtual void ChangeEnd(GameScene* _pNextScene)	{} 
		//����� �ٲ�鼭 ���� ��鿡�� ������ ���ְ� �ʹٸ� �Ʒ� �Լ��� ����Ѵ�.
		//��Ÿ�ວ������ ����� �ٲ��� �۹��� �ڶ�� �� ���� ����.
		virtual void ChangeStart(GameScene* _pPrevScene) {} //����� ���۵ɶ� �������� �Լ� 
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

	///////////Actor ����/////////////
public:
	std::list<SPtr<GameActor>> m_AllActor;
	std::map<GameString, SPtr<GameActor>> m_FindActor;
	std::list<SPtr<GameActor>>::iterator m_StartIter;
	std::list<SPtr<GameActor>>::iterator m_EndIter;

	//////////�������///////////////////
private:
	static std::map<GameString, SPtr<GameScene>> m_AllMap;

	static SPtr<GameScene> m_CurScene;
	static SPtr<GameScene> m_NextScene;

	bool m_IsLoading;
	//����� ������Ʈ�� ���
	std::list<SPtr<SceneComponent>> m_AllSceneCom;
	std::list<SPtr<SceneComponent>>::iterator m_AllSceneComStartIter;
	std::list<SPtr<SceneComponent>>::iterator m_AllSceneComEndIter;

public:
	//�������� ���
	GameRenderMgr RENDERMGR;
	//�ݸ����� ���
	GameColMgr COLMGR;

public:
	GameScene();
	~GameScene();

public:
	static SPtr<GameScene> FindScene(const GameString& _Name);
	static SPtr<GameScene> CreateScene(const GameString& _Name);

	//CreateScene<������Ʈ�ڷ���> ���鶧 ������Ʈ�� �ڵ����� �־���
	template<typename T, typename ... Rest>
	static SPtr<GameScene> CreateScene(const GameString& _Name)
	{
		SPtr<GameScene> NewScene = CreateScene(_Name);
		NewScene->AddCom<T>();
		return NewScene;
	}

	static void ChangeScene(const GameString& _Name);

	//������ ���� update�� ��������� �Լ�
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

	//////Actor ���� �Լ�
	//////Component�� CreateActor�� �̰� ȣ���Ѵ�.
	//�⺻�� �̸��� ������ �װɷ� ã�� �� �ְ� �ƴϸ� Update�� ����.
	SPtr<GameActor> CreateActor(const GameString& _Name = L"");
	SPtr<GameActor> FindActor(const GameString& _Name);

	//Update �Լ�
	void UpdatePrev();
	void Update();
	void UpdateNext();
	//������ �Լ�, RenderMGR�� Render()�� ȣ�����ش�.
	void Render();
	void Collision();
	void DebugRender();
	void Release();

private:
	//��鿡�� ���� ����� �Ѿ�� ���� �ϰ� ������
	void ChangeEnd(GameScene* _pNextScene);
	void ChangeStart(GameScene* _pPrevScene);
};

