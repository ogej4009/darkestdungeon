#include "GameAni.h"
#include "GameImage.h"
#include <GameTime.h>
#include <GameTFunc.h>


GameAni::S2DANIDATA::S2DANIDATA()
	: m_Cur(0),
	m_Start(0),
	m_End(0),
	m_InterTime(0),
	m_CurTime(0),
	m_bLoop(false)
{

}

GameAni::S2DANIDATA::~S2DANIDATA()
{

}

void GameAni::S2DANIDATA::Reset()
{
	m_Cur = m_Start;
	m_CurTime = m_InterTime;
	m_bEnd = false;
}

void GameAni::S2DANIDATA::Update()
{
	m_CurTime -= GameTime::DeltaTime();

	if (0 >= m_CurTime)
	{
		++m_Cur;
		
		//�տ��� ������� �̺�Ʈ�� ������ �Լ�
		//������ ���������� Ű�� ������ �ִ´�.
		FindStartEventIter = m_StartEventList.find(m_Cur);
		if (FindStartEventIter != m_StartEventList.end())
		{
			for (auto& event : FindStartEventIter->second)
			{
				event(m_Parent);
			}
		}

		FindMemberStartEventIter = m_StartMemberEventList.find(m_Cur);
		if (FindMemberStartEventIter != m_StartMemberEventList.end())
		{
			for (auto& event : FindMemberStartEventIter->second)
			{
				//std::list<std::function<void()>>�� ����� �ϳ��� ������
				event();
			}
		}

		m_CurTime = m_InterTime;
		//������ ��ŸƮ �������� �ٲ㼭 ��� ����
		if (true == m_bLoop)
		{
			if (m_End < m_Cur)
			{
				m_Cur = m_Start;
			}
		}
		else
		{
			//������ �ƴϸ� ������ ��������Ʈ���� ������
			if (m_End < m_Cur)
			{
				m_bEnd = true;
				--m_Cur;
			}
		}
	}
}

////////// GameAni

void GameAni::Init()
{
	m_ImgRender = Actor()->GetCom<GameImage>();

	if (nullptr == m_ImgRender)
	{
		ASSERTMSG;
	}
}

SPtr<GameAni::S2DANIDATA> GameAni::FindAni(const GameString& _AniName)
{
	return GameTFunc::MapFindPointer(m_AllAni, _AniName);
}

void GameAni::ChangeAni(const GameString& _AniName, bool _Loop /*= true*/, size_t _StartIndex /*= -1*/)
{
	m_CurAni = FindAni(_AniName);
	m_CurAni->Reset();
	m_CurAni->m_bLoop = _Loop;

	if (-1 != _StartIndex)
	{
		m_CurAni->m_Cur = _StartIndex;
	}
	//�ƴ� ��쿡�� 0���� �ʱ�ȭ

	if (nullptr == m_CurAni)
	{
		ASSERTMSG;
	}
}

void GameAni::CreateAni(const GameString& _AniName, const GameString& _SpriteName, size_t _Start, size_t _End, float _Time, bool _Loop)
{
	//���� �̸��� �ִϸ��̼� ���� �Ұ�
	if (nullptr != FindAni(_AniName))
	{
		ASSERTMSG;
	}

	S2DANIDATA* NewAni = new S2DANIDATA();

	NewAni->m_AniName = _AniName;
	NewAni->m_SpriteName = _SpriteName;
	NewAni->m_InterTime = _Time;
	NewAni->m_Start = _Start;
	NewAni->m_End = _End;
	NewAni->m_bLoop = _Loop;
	NewAni->m_bEnd = false;
	NewAni->m_Parent = this;

	m_AllAni.insert(std::make_pair(_AniName, NewAni));
}

void GameAni::UpdateNext()
{
	m_CurAni->Update();

	//�̹��� �������� ���� ���� �÷��ַ�
	m_ImgRender->Index(m_CurAni->m_Cur);
	//�̹��� �������� �� �̸��� ��������Ʈ�� �������ַ�
	m_ImgRender->Sprite(m_CurAni->m_SpriteName);
}

void GameAni::AddStartEvent(const GameString& _AniName, size_t _Frame, void(*_Func)(GameLogic* _Parent))
{
	// �̰� �����Ƽ� �ȸ�����µ�
	// �� �ִϸ��̼��� ��ŸƮ�� �ص� �ٱ��� �ִ� �������̸� �����ε�
	// �׳���.

	S2DANIDATA* FAni = FindAni(_AniName);

	std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::iterator FAniIter = FAni->m_StartEventList.find(_Frame);

	if (FAni->m_StartEventList.end() == FAniIter)
	{
		//Logic������ �Ű������� �ϳ� �޴� ������ �Լ���� � ������ �޾��� �� �ִ� ������ ����Ʈ�� �ִϸ��̼� �����Ͱ� ��� �ִ�.
		FAni->m_StartEventList.insert(std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::value_type(_Frame, std::list<void(*)(GameLogic* _Parent)>()));
		FAniIter = FAni->m_StartEventList.find(_Frame);
	}

	FAniIter->second.push_back(_Func);

}
