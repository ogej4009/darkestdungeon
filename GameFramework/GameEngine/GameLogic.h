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
	//���� ������Ʈ���� �θ� ����->�� ���·� �Ѿ�� ���ο� ���ͳ� ������Ʈ�� ������ �� �ִ�.
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


