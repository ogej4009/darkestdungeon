#pragma once
#include <GameLogic.h>
#include <GameString.h>
#include <LogicHeader.h>

class GameAni;
class HeroFsm : public GameLogic
{
private:
	float m_Speed;
	bool b_Can_Left_Move;
	bool b_Can_Right_Move;
	SPtr<GameAni> m_Ani;
	GameString m_HeroName;
	std::vector<GameString> m_FSMList;
	HeroFSMLIST m_CurFSM;
	HeroFSMLIST m_NextFSM;
	bool b_NextFSM_Loop;
	bool b_Is_Battle;
	float m_FSMTimer;

public:
	HeroFsm();
	~HeroFsm();

public:
	void Init(const GameString& _HeroName);
	void Update() override;
	void DebugRender() override;

public:
	void InputFSMName(const GameString& _FSMName);
	void BattleStart()
	{
		b_Is_Battle = true;
	}

	void BattleEnd()
	{
		ChangeFSM(HeroFSMLIST::Idle);
		b_Is_Battle = false;
	}

public:
	void HeroWalk();
	void StopMove(bool _IsRight);
	void CanMove(bool _IsRight);
	void CheckFSM();
	void ChangeFSM(HeroFSMLIST _FSM, bool _Loop = true);
};

