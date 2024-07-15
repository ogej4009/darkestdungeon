#include "GameCol.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameColMgr.h"
//Bounding Box Ȱ���� ���ؼ� �ʿ��� ���
#include <DirectXCollision.h>
#include <DirectXCollision.inl>
//�и��� �̷��� �̿��ؼ� ���а��� ������ Ȯ���Ѵٰ� �Ѵ�.
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
		//�浹�� �ߴµ� �� �浹 ����Ʈ�� ���� ���̶� �浹�Ѱ�� == Enter
		if (m_OtherCol.end() == m_OtherCol.find(_Col))
		{
			//������ �� �ְ� �ʲ��� �� ����
			m_OtherCol.insert(_Col);
			_Col->m_OtherCol.insert(this);

			for (auto& _Event : m_EnterMemberEventList)
			{
				//�ش� ���� �̺�Ʈ ����Ʈ���� �����Լ� or ����Լ� ������ ����ǹǷ�
				//ȣ��ÿ� �Ʒ��� ���� ȣ���ϰ� �ȴ�.
				_Event(this, _Col);
			}
		}
		//�浹�ߴµ� �� �浹 ����Ʈ�� �����Ѵ� == Stay
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
		//�浹���� �ƴѵ� �浹����Ʈ�� �������� �ִٰ�? == Exit
		if (m_OtherCol.end() != m_OtherCol.find(_Col))
		{
			for (auto& _Event : m_ExitMemberEventList)
			{
				_Event(this, _Col);
			}

			//������ Colüũ����Ʈ���� ��������.
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

//�浹ü�� �������� ��� �� otherCol���� ã�Ƽ� ���ڸ� �����ּ���.
void GameCol::Release()
{
	for (auto& _Col : m_OtherCol)
	{
		_Col->m_OtherCol.erase(this);
	}

}