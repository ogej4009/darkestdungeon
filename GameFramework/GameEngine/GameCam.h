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

	float	m_Near; // ����� �Ÿ�
	float	m_Far; // ����� �Ÿ�
	CVector   m_Size;
	float	m_Fov; // �þ߰�

	CMatrix	m_MatView;
	CMatrix	m_MatProj;
	CMatrix	m_MatViewProj;
	CAMTYPE m_Type;

	//���� m_Order��°�� ������ �Ѵ�
	unsigned int m_Order;

	//ī�޶� ���� ���� �׷�
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


	// Cam�ʱ�ȭ�� �Ű������� _Order �ϳ��϶�
	void Init(unsigned int _DrawOrder);

	//�������� ���ø�,�Ű������� 0���� �ɶ����� ��������� ȣ��ȴ�.
	//�Ű������� ������ �׷��� �޴´�.
	template<typename ... Rest>
	void Init(unsigned int _Order, Rest ... _Data)
	{
		//ó�� �Ű������� DrawOrder�� ������
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

	//��ǥ��ȯ �Լ�
	CVector ScreenPos3DToWorldPos(const CVector& _ScreenPos);

	//Component�� ���� UpdateNext�� 
	//���ؽ� ������ ������Ʈ �� ���Ŀ� ���� �ϱ� ���ؼ�
	//�ؿ�ó�� Next�� �����
	void UpdateNext() override;
};

