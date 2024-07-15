#include "MonsterFsm.h"
#include <GameAni.h>
#include <GameTime.h>

MonsterFsm::MonsterFsm()
{

}

MonsterFsm::~MonsterFsm()
{

}

void MonsterFsm::Init(const GameString& _MonsterName)
{
	m_Ani = Actor()->GetCom<GameAni>();
	m_MonsterName = _MonsterName;
}

void MonsterFsm::Update()
{
	m_FSMTimer -= GameTime::DeltaTime();
	CheckFSM();


	if (m_FSMTimer < 0 && MonsterFSMLIST::Combat != m_CurFSM)
	{
		ChangeFSM(MonsterFSMLIST::Combat);
	}
}

void MonsterFsm::InputFSMName(const GameString& _FSMName)
{
	m_FSMList.push_back(_FSMName);
}


void MonsterFsm::CheckFSM()
{
	if (m_NextFSM != MonsterFSMLIST::MAX)
	{
		m_Ani->ChangeAni(m_FSMList[(int)m_NextFSM], b_NextFSM_Loop);
		m_CurFSM = m_NextFSM;
		m_NextFSM = MonsterFSMLIST::MAX;
	}
}

void MonsterFsm::ChangeFSM(MonsterFSMLIST _FSM, bool _Loop /*= true*/)
{
	m_NextFSM = _FSM;
	b_NextFSM_Loop = _Loop;
	m_FSMTimer = 1.2f;
}