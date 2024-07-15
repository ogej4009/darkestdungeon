#pragma once
#include <GameLogic.h>
#include <GameString.h>
#include <LogicHeader.h>

class GameAni;
class MonsterFsm : public GameLogic
{
private:
	SPtr<GameAni> m_Ani;
	GameString m_MonsterName;
	std::vector<GameString> m_FSMList;
	MonsterFSMLIST m_CurFSM;
	MonsterFSMLIST m_NextFSM;
	bool b_NextFSM_Loop;
	bool b_Is_Battle;
	float m_FSMTimer;

public:
	MonsterFsm();
	~MonsterFsm();

public:
	void Init(const GameString& _MonsterName);
	void Update() override;

public:
	void InputFSMName(const GameString& _FSMName);
	void CheckFSM();
	void ChangeFSM(MonsterFSMLIST _FSM, bool _Loop = true);
};

