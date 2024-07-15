#pragma once
#include <GameLogic.h>

class GameCol;
class GameAni;
class Hero : public GameLogic
{
private:
	int AttPoint;

	GameString m_StateName;
	SPtr<GameCol> COL;

	float m_Speed;
	SPtr<GameAni> ANICOM;

public:
	Hero();
	~Hero();

public:
	void Init() override;
	void Update() override;
	void DebugRender() override;


public:
	bool IsState(const GameString& _StateName);
	void StateChange(const GameString& _StateName);
	static void GEnterEvent(GameCol* _This, GameCol* _Other);
	void EnterEvent(GameCol* _This, GameCol* _Other);
	void FootPrint(GameLogic* _Logic);
	void Walk();
	void Att();
};

