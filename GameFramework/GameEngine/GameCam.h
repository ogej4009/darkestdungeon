#pragma once
#include "GameComponent.h"
#include "GameDevice.h"
#include <list>

class GameRenderMgr;
class GameCam :
	public GameComponent
{
public:
	enum CAMTYPE
	{
		PERS,
		ORTH,
	};

private:
	friend GameRenderMgr;

	float	m_Near; // 근평면 거리
	float	m_Far; // 원평면 거리
	CVector   m_Size;
	float	m_Fov; // 시야각

	CMatrix	m_MatView;
	CMatrix	m_MatProj;
	CMatrix	m_MatViewProj;
	CAMTYPE m_Type;

	//나는 m_Order번째로 랜더를 한다
	unsigned int m_Order;

	//카메라가 보고 있을 그룹
	std::list<unsigned int> m_Group;

public:
	GameCam();
	~GameCam();

public:
	void ViewType(CAMTYPE _Type)
	{
		m_Type = _Type;
	}

	unsigned int Order()
	{
		return m_Order;
	}

	void Size(CVector _Size)
	{
		m_Size = _Size;
	}

	CVector Size()
	{
		return m_Size;
	}


	// Cam초기화시 매개변수가 _Order 하나일때
	void Init(unsigned int _DrawOrder);

	//가변인자 템플릿,매개변수가 0개가 될때까지 재귀적으로 호출된다.
	//매개변수는 랜더링 그룹을 받는다.
	template<typename ... Rest>
	void Init(unsigned int _Order, Rest ... _Data)
	{
		//처음 매개변수는 DrawOrder로 고정됨
		Init(_Order);

		PushRenderGroup(_Data...);
	}

	template<typename ... Rest>
	void PushRenderGroup(unsigned int _GroupOrder, Rest ... _Data)
	{
		m_Group.push_back(_GroupOrder);
		PushRenderGroup(_Data...);
	}

	void PushRenderGroup(unsigned int _GroupOrder)
	{
		m_Group.push_back(_GroupOrder);
	}

	CMatrix MatView()
	{
		return m_MatView;
	}

	CMatrix MatProj()
	{
		return m_MatProj;
	}

	CMatrix MatViewProj()
	{
		return m_MatViewProj;
	}

	//좌표변환 함수
	CVector ScreenPos3DToWorldPos(const CVector& _ScreenPos);

	//Component를 위한 UpdateNext로 
	//버텍스 정보가 업데이트 된 이후에 돌게 하기 위해서
	//밑에처럼 Next를 사용함
	void UpdateNext() override;
};

