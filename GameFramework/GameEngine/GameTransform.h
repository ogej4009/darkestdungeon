#pragma once
#include "GameDevice.h"
#include "GameComponent.h"
#include <GameTime.h>

class GameCam;
class GameTransform : public GameComponent
{
private:
	GameTransform* m_pParent;

	CVector	m_LScale;
	CVector	m_LRotQ;
	CVector	m_LRot;
	CVector	m_LPos;

	CVector	m_WScale;
	CVector	m_WRotQ;
	CVector	m_WRot;
	CVector	m_WPos;

	CMatrix	m_MatScale;
	CMatrix	m_MatRotX;
	CMatrix	m_MatRotY;
	CMatrix	m_MatRotZ;
	CMatrix	m_MatRot;
	CMatrix	m_MatPos;
	CMatrix	m_LWorld;
	CMatrix	m_WWorld;

protected:
	//Render에서 사용
	CTransData m_TransData;

public:
	GameTransform();
	~GameTransform();

public:
	const CMatrix& CREFWW()
	{
		return m_WWorld;
	}

	void LMOVETOTIMESPEED(CVector _Value, float _Speed)
	{
		LMOVE(_Value * GameTime::DeltaTime() * _Speed);
	}

	void LMOVETOTIME(CVector _Value)
	{
		LMOVE(_Value * GameTime::DeltaTime());
	}

	void LMOVE(CVector _Value)
	{
		m_LPos += _Value;
	}


	//방향 단위벡터를 받고 각 방향으로 이동하게 조정하는 함수
	void Move(CVector _Value)
	{
		m_LPos += _Value;
	}

	void MoveToTime(CVector _Value)
	{
		Move(_Value * GameTime::DeltaTime());
	}

	void MoveToTimeSpeed(CVector _Value, float _Speed)
	{
		Move(_Value * GameTime::DeltaTime() * _Speed);
	}

	//로컬 포스를 리턴함
	CVector LPOS()
	{
		return m_LPos;
	}

	void LPOS(CVector _Value)
	{
		m_LPos = _Value;
	}

	void LSCALE(CVector _Value)
	{
		m_LScale = _Value;
	}

	CVector LSCALE() {
		return m_LScale;
	}

	void LROT(CVector _Value)
	{
		m_LRot = _Value;
	}

	CVector LROT()
	{
		return m_LRot;
	}

	void WPOS(CVector _Value)
	{
		if (nullptr == m_pParent)
		{
			m_WPos = _Value;
			m_LPos = _Value;
			return;
		}

		CVector PScale = m_pParent->WSCALE();
		CVector PPos = _Value - m_pParent->WPOS();

		m_WPos = _Value;
		m_LPos =
		{
			PScale.x != 0.0f ? PPos.x / PScale.x : PPos.x,
			PScale.y != 0.0f ? PPos.y / PScale.y : PPos.y,
			PScale.z != 0.0f ? PPos.z / PScale.z : PPos.z
		};
	}

	CVector WPOS()
	{
		return m_WPos;
	}

	CVector LQ()
	{
		return m_LRotQ;
	}

	CVector WQ()
	{
		return m_WRotQ;
	}

	void WSCALE(CVector _Value)
	{
		if (nullptr == m_pParent)
		{
			m_WScale = _Value;
			m_LScale = _Value;
			return;
		}

		CVector PScale = m_pParent->WSCALE();

		m_WScale = _Value;
		m_LScale = {
			PScale.x != 0.0f ? _Value.x / PScale.x : _Value.x,
			PScale.y != 0.0f ? _Value.y / PScale.y : _Value.y,
			PScale.z != 0.0f ? _Value.z / PScale.z : _Value.z
		};
	}

	CVector WSCALE()
	{
		return m_WScale;
	}

	void WROT(CVector _Value)
	{
		if (nullptr == m_pParent)
		{
			m_WRot = _Value;
			m_LRot = _Value;
			return;
		}

		CVector PRot = m_pParent->WROT();

		m_WRot = _Value;
		m_LRot = {
			_Value.x - PRot.x,
			_Value.y - PRot.y,
			_Value.z - PRot.z,
		};
		QSetting();
	}

	CVector WROT()
	{
		return m_WRot;
	}

	//월드에서의 앞,뒤,좌,우,위,아래값을 각각 리턴함
	CVector WFORWARD()
	{
		return m_WWorld.ArrVec[2].Normal3DReturn();
	}

	CVector WBACK()
	{
		return -WFORWARD();
	}

	CVector WRIGHT()
	{
		return m_WWorld.ArrVec[0].Normal3DReturn();
	}

	CVector WLEFT()
	{
		return -WRIGHT();
	}

	CVector WUP()
	{
		return m_WWorld.ArrVec[1].Normal3DReturn();
	}

	CVector WDOWN()
	{
		return -WUP();
	}

	void ScaleXMinus()
	{
		if (0 < m_LScale.x)
		{
			m_LScale.x = -m_LScale.x;
		}
	}

	void ScaleXPlus()
	{
		if (0 > m_LScale.x)
		{
			m_LScale.x = m_LScale.x * -1.0f;
		}
	}

	void LROTZ(float _Value) { m_LRot.z += _Value; }
	void WROTZ(float _Value) { m_LRot.z += _Value; }
	void LROTX(float _Value) { m_LRot.x += _Value; }
	void WROTX(float _Value) { m_LRot.x += _Value; }
	void LROTY(float _Value) { m_LRot.y += _Value; }
	void WROTY(float _Value) { m_LRot.y += _Value; }

	//생성시에 World를 어떤 함수로 할지 선택하게 되는 함수 포인터
	void (GameTransform::*WorldFunc)();

	void Parent(GameTransform* _pParent)
	{
		if (nullptr == _pParent)
		{
			if (nullptr != m_pParent)
			{

			}
			else
			{
				WorldFunc = &GameTransform::CalWorld;
			}
		}

		m_pParent = _pParent;
		WorldFunc = &GameTransform::CalParentWorld;
	}

	GameTransform* Parent()
	{
		return m_pParent;
	}
	
	void Init();
	void Update();
	void CalTransData(GameCam* _Cam);

private:
	void CalWorld();
	void CalParentWorld();
	void QSetting();
};

