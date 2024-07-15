#pragma once
#include "GameObjRef.h"
#include <GameName.h>
#include <list>

//���߻���� �Ҷ��� �������� ���� Ŭ����(�� ��� Ưȭ)�� �θ�� ����ϰ�
//���̾Ƹ�� ������ �����ؾ��� 
//������ ������Ʈ���� GameComponent list�� �ᵵ ������ �����µ�
//���ʿ��� �ش� ������ ���ܼ� SGAMECOM�� �߰�����

class GameTransform;
class GameScene;
class GameComponent;
class GameActor final : public GameObjRef, public GameName
{
	//���Ͱ� ����� ������ ���� �ƴϰ� ������Ʈ ������
	//���� ������Ʈ�� ������� Ư�� ������ �����ϰ� �Ѵ�.
	//��Ӱ��� �ٸ� ����ü���̶�� ���� �ɵ�

private:
	friend GameScene;
	bool m_IsFind;
	GameScene* m_pScene;
	std::list<SPtr<GameComponent>> m_AllCom;
	std::list<SPtr<GameComponent>>::iterator m_StartIter;
	std::list<SPtr<GameComponent>>::iterator m_EndIter;
	GameTransform* m_pTrans;


public:
	GameActor();
	~GameActor();

public:
	void ChangeScene(GameString& _Name);
	void UpdatePrev();
	void Update();
	void UpdateNext();
	void DebugRender();

	// �������ڷ� ������Ʈ�� �Ű����� �������� ���� �� �ְ���
	// �� �޴��ʿ� ������ �Ű������� ���� �Լ��� �־�߸� ��� ��������
	template<typename T, typename ...ARG>
	SPtr<T> AddCom(ARG ... _Data)
	{
		T* NewCom = new T();
		const type_info* TInfo = &typeid(T);
		NewCom->TypeSetting(TInfo);
		NewCom->m_pActor = this;
		NewCom->Init(_Data ...);
		m_AllCom.push_back(NewCom);
		return NewCom;
	}

	template<typename TYPE>
	SPtr<TYPE> GetCom()
	{
		const type_info* TInfo = &typeid(TYPE);
		for (auto & value : m_AllCom)
		{
			if (true == value->Check(TInfo))
			{
				return value;
			}
		}
		return nullptr;
	}

	template<typename TYPE>
	bool HasCom()
	{
		const type_info* TInfo = &typeid(TYPE);
		for (auto & value : m_AllCom)
		{
			if (true == value->Check(TInfo))
			{
				return true;
			}
		}
		return false;
	}

	GameScene* Scene()
	{
		return m_pScene;
	}

	GameTransform* Trans()
	{
		return m_pTrans;
	}

	void DeleteCom();
};
