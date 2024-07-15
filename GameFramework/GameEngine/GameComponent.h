#pragma once
#include <GameName.h>
#include <GameRTTI.h>
#include <list>
#include "GameActor.h"

//컴포넌트 구조의 컴포넌트를 의미한다.
class GameTransform;
class GameComponent : public GameObjRef, public GameName, public GameRTTI
{
public:
	friend GameActor;

private:
	GameActor* m_pActor;


public:
	GameComponent();
	~GameComponent();

public:
	virtual void Init();

	//Scene이 Actor에서 함수를 호출하고 Actor가 컴포넌트의 업데이트를 호출함 
	//Scene->Actor->Com 순으로 Update해주는 함수들
	virtual void UpdatePrev() {}
	virtual void Update() {}
	virtual void UpdateNext() {}
	virtual void DebugRender() {}

public:
	GameActor* Actor()
	{
		return m_pActor;
	}

	//기본으로 Actor와 그 컴포넌트는 Trans를 넣어주기로 했음
	GameTransform* Trans();

	//부모 자식관계의 근본은 Actor와 Component의 관계이므로
	//여기서 죽음 체크를 오버라이딩 한다.

	bool IsDeath() override
	{
		//앞쪽은 액터의 죽음이고 뒤쪽은 컴포넌트의 죽음이다.
		//부모 액터가 죽었다면 나도 죽은거로 쳐라
		return Actor()->IsDeath() || GameObjRef::IsDeath();
	}

	bool IsUpdate() override
	{
		//부모도 업데이트 해야하고 나도 업데이트 해야한다면 업데이트 해
		return Actor()->IsUpdate() && GameObjRef::IsUpdate();
	}

	//부모도 On이고 나도 On이면 On이야
	bool IsOn() override
	{
		return Actor()->IsOn() && GameObjRef::IsOn();
	}

};
