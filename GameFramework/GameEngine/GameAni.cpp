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
		
		//앞에서 만들어준 이벤트를 돌리는 함수
		//프레임 숫자정보를 키로 가지고 있는다.
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
				//std::list<std::function<void()>>의 멤버를 하나씩 꺼냈음
				event();
			}
		}

		m_CurTime = m_InterTime;
		//루프면 스타트 지점으로 바꿔서 계속 진행
		if (true == m_bLoop)
		{
			if (m_End < m_Cur)
			{
				m_Cur = m_Start;
			}
		}
		else
		{
			//루프가 아니면 마지막 스프라이트에서 정지해
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
	//아닌 경우에는 0으로 초기화

	if (nullptr == m_CurAni)
	{
		ASSERTMSG;
	}
}

void GameAni::CreateAni(const GameString& _AniName, const GameString& _SpriteName, size_t _Start, size_t _End, float _Time, bool _Loop)
{
	//같은 이름의 애니메이션 생성 불가
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

	//이미지 랜더러야 지금 컷을 올려주렴
	m_ImgRender->Index(m_CurAni->m_Cur);
	//이미지 랜더러야 이 이름의 스프라이트를 세팅해주렴
	m_ImgRender->Sprite(m_CurAni->m_SpriteName);
}

void GameAni::AddStartEvent(const GameString& _AniName, size_t _Frame, void(*_Func)(GameLogic* _Parent))
{
	// 이거 귀찮아서 안만들었는데
	// 이 애니메이션의 스타트와 앤드 바깥에 있는 프레임이면 에러인데
	// 그냥함.

	S2DANIDATA* FAni = FindAni(_AniName);

	std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::iterator FAniIter = FAni->m_StartEventList.find(_Frame);

	if (FAni->m_StartEventList.end() == FAniIter)
	{
		//Logic포인터 매개변수를 하나 받는 형태의 함수라면 어떤 종류든 받아줄 수 있는 형태의 리스트를 애니메이션 데이터가 들고 있다.
		FAni->m_StartEventList.insert(std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::value_type(_Frame, std::list<void(*)(GameLogic* _Parent)>()));
		FAniIter = FAni->m_StartEventList.find(_Frame);
	}

	FAniIter->second.push_back(_Func);

}
