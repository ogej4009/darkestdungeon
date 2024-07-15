#include "HeroFsm.h"
#include <GameInput.h>
#include <GameAni.h>

HeroFsm::HeroFsm() : m_Speed(2.0f), b_Can_Left_Move(true), b_Can_Right_Move(true), m_NextFSM(HeroFSMLIST::MAX), b_NextFSM_Loop(true), b_Is_Battle(false)
{

}

HeroFsm::~HeroFsm()
{

}

void HeroFsm::Init(const GameString& _HeroName)
{
	if (GameInput::IsKey(L"LEFTMOVE") == false)
	{
		GameInput::CreateKey(L"LEFTMOVE", 'A');
		GameInput::CreateKey(L"RIGHTMOVE", 'D');
	}
	m_Ani = Actor()->GetCom<GameAni>();
	m_HeroName = _HeroName;

}

void HeroFsm::Update()
{
	if (m_FSMTimer > 0)
	{
		m_FSMTimer -= GameTime::DeltaTime();
	}

	if (false == b_Is_Battle)
	{
		HeroWalk();
	}
	else if (m_CurFSM != HeroFSMLIST::Combat && m_FSMTimer < 0)
	{
		ChangeFSM(HeroFSMLIST::Combat);
	}

	CheckFSM();
}

void HeroFsm::DebugRender()
{

}

void HeroFsm::HeroWalk()
{
	if (true == b_Can_Left_Move)
	{
		if (true == GameInput::Down(L"LEFTMOVE"))
		{
			ChangeFSM(HeroFSMLIST::Walk);
		}

		if (true == GameInput::Press(L"LEFTMOVE"))
		{
			Trans()->MoveToTimeSpeed(CVector::LEFT, m_Speed);
		}
	}

	if (true == b_Can_Right_Move)
	{
		if (true == GameInput::Down(L"RIGHTMOVE"))
		{
			ChangeFSM(HeroFSMLIST::Walk);
		}

		if (true == GameInput::Press(L"RIGHTMOVE"))
		{
			Trans()->MoveToTimeSpeed(CVector::RIGHT, m_Speed);
		}
	}

	if (true == GameInput::Up(L"LEFTMOVE") || true == GameInput::Up(L"RIGHTMOVE"))
	{
		ChangeFSM(HeroFSMLIST::Idle);
	}
}

void HeroFsm::StopMove(bool _IsRight)
{
	if (_IsRight == true)
	{
		b_Can_Right_Move = false;
	}
	else
	{
		b_Can_Left_Move = false;
	}
}


void HeroFsm::CanMove(bool _IsRight)
{
	if (_IsRight == true)
	{
		b_Can_Right_Move = true;
	}
	else
	{
		b_Can_Left_Move = true;
	}
}

void HeroFsm::InputFSMName(const GameString& _FSMName)
{
	m_FSMList.push_back(_FSMName);
}

void HeroFsm::ChangeFSM(HeroFSMLIST _FSM, bool _Loop /*= true*/)
{
	m_NextFSM = _FSM;
	b_NextFSM_Loop = _Loop;
	m_FSMTimer = 1.2f;
}

void HeroFsm::CheckFSM()
{
	if (m_NextFSM != HeroFSMLIST::MAX)
	{
		m_Ani->ChangeAni(m_FSMList[(int)m_NextFSM], b_NextFSM_Loop);
		m_CurFSM = m_NextFSM;
		m_NextFSM = HeroFSMLIST::MAX;
	}
}