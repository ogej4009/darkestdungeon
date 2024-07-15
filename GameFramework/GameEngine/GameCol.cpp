#include "GameCol.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameColMgr.h"
//Bounding Box 활용을 위해서 필요한 헤더
#include <DirectXCollision.h>
#include <DirectXCollision.inl>
//분리축 이론을 이용해서 선분간의 접촉을 확인한다고 한다.
#include "GameDebugEn.h"

GameCol::GameCol()
{
}


GameCol::~GameCol()
{
}

void GameCol::Init(SCOLTYPE _Type, unsigned int _Order /*= 0*/)
{
	m_Order = _Order;

	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	DebugOn();

	Parent(Actor()->Trans());
	Type(_Type);
	Actor()->Scene()->COLMGR.PushCol(this);
}

void GameCol::Collision(GameCol* _Col)
{
	if (true == ColCheck(_Col))
	{
		//충돌을 했는데 내 충돌 리스트에 없던 놈이랑 충돌한경우 == Enter
		if (m_OtherCol.end() == m_OtherCol.find(_Col))
		{
			//내꺼에 너 넣고 너꺼에 나 넣자
			m_OtherCol.insert(_Col);
			_Col->m_OtherCol.insert(this);

			for (auto& _Event : m_EnterMemberEventList)
			{
				//해당 엔터 이벤트 리스트에는 전역함수 or 멤버함수 명으로 저장되므로
				//호출시에 아래와 같이 호출하게 된다.
				_Event(this, _Col);
			}
		}
		//충돌했는데 내 충돌 리스트에 존재한다 == Stay
		else
		{
			for (auto& _Event : m_StayMemberEventList)
			{
				_Event(this, _Col);
			}
		}
	}
	else
	{
		//충돌중이 아닌데 충돌리스트에 누군가가 있다고? == Exit
		if (m_OtherCol.end() != m_OtherCol.find(_Col))
		{
			for (auto& _Event : m_ExitMemberEventList)
			{
				_Event(this, _Col);
			}

			//서로의 Col체크리스트에서 지워주자.
			m_OtherCol.erase(_Col);
			_Col->m_OtherCol.erase(this);
		}
	}
}

bool GameCol::ColCheck(GameCol* _Col)
{
	pData->DataUpdate(this);
	_Col->pData->DataUpdate(_Col);

	return pData->ColCheck(_Col->pData);
}

void GameCol::Type(SCOLTYPE _Type)
{
	switch (_Type)
	{
	case GameCol::SCOLTYPE::AABBRECT:
		pData = new COLAABBBOX();
		pData->Type = SCOLTYPE::AABBRECT;
		break;
	case GameCol::SCOLTYPE::OBBRECT:
		pData = new COLOBBBOX;
		pData->Type = SCOLTYPE::OBBRECT;
		break;
	case GameCol::SCOLTYPE::CIRCLE:
		pData = new COLCIRCLE();
		pData->Type = SCOLTYPE::CIRCLE;
		break;
	default:
		break;
	}
}

void GameCol::DebugRender()
{
	GameDebugEn::Draw2DRect(this);
}

//충돌체가 지워지는 경우 내 otherCol에서 찾아서 각자를 지워주세요.
void GameCol::Release()
{
	for (auto& _Col : m_OtherCol)
	{
		_Col->m_OtherCol.erase(this);
	}

}