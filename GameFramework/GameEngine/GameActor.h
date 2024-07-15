#pragma once
#include "GameObjRef.h"
#include <GameName.h>
#include <list>

//다중상속을 할때는 가능한한 작은 클래스(한 기능 특화)를 부모로 써야하고
//다이아몬드 구조를 조심해야함 
//선생님 프로젝트에선 GameComponent list를 써도 문제가 없었는데
//내쪽에선 해당 문제가 생겨서 SGAMECOM을 추가해줌

class GameTransform;
class GameScene;
class GameComponent;
class GameActor final : public GameObjRef, public GameName
{
	//액터가 상속을 내리는 것이 아니고 컴포넌트 구조로
	//가진 컴포넌트의 기능으로 특정 역할을 수행하게 한다.
	//상속과는 다른 구조체험이라고 보면 될듯

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

	// 가변인자로 컴포넌트의 매개변수 여러개를 받을 수 있게함
	// 단 받는쪽에 동일한 매개변수를 쓰는 함수가 있어야만 사용 가능해짐
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
