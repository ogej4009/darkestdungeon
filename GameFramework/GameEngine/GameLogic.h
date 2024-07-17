#pragma once
#include "GameComponent.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameCol.h"

class GameTransform;
class GameLogic : public GameComponent
{
public:
	GameLogic() {}
	~GameLogic() {}

public:
	//로직 컴포넌트에선 부모 액터->신 형태로 넘어가서 새로운 액터나 컴포넌트를 생성할 수 있다.
	template<typename TYPE>
	SPtr<TYPE> GetCom()
	{
		return Actor()->GetCom<TYPE>();
	}

	template<class TYPE, typename ...ARG>
	SPtr<TYPE> AddCom(ARG ... _Data)
	{
		return Actor()->AddCom<TYPE>(_Data...);
	}

	SPtr<GameActor> CreateActor(const GameString& _Name)
	{
		return Actor()->Scene()->CreateActor(_Name);
	}

	SPtr<GameActor> CreateActor()
	{
		return Actor()->Scene()->CreateActor();
	}
};


