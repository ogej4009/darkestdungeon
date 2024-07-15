#include "GameCam.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameTransform.h"


GameCam::GameCam()
	: m_Near(0.1f)
	, m_Far(10000.0f)
	, m_Size(GameWin::MainWindow()->Size())
	, m_Fov(60.0f)
	, m_Type(CAMTYPE::ORTH)
{
}


GameCam::~GameCam()
{
}


void GameCam::Init(unsigned int _DrawOrder)
{
	m_Order = _DrawOrder;

	//��ġ������ �� �־����, ī�޶�ϱ�
	if (nullptr == Actor()->GetCom<GameTransform>())
	{
		ASSERTMSG;
	}

	//Actor���� Cam�� �ϳ����� ����ִ´�.
	if (nullptr != Actor()->GetCom<GameCam>())
	{
		ASSERTMSG;
	}

	//������ ���� ���°͵� �ȵ�
	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	Actor()->Scene()->RENDERMGR.PushCam(this);
}

CVector GameCam::ScreenPos3DToWorldPos(const CVector& _ScreenPos)
{
	// m_Size �������� ����� �ʿ��ϴ�.
	// m_Size ���� ������ ȭ���� ���� ������

	CVector WinScale = GameWin::MainWindow()->Size();
	// ������ ���� 0,0
	// 1280 720
	// 16, 9

	CVector ConPos;
	ConPos.x = _ScreenPos.x * m_Size.x / WinScale.x;
	ConPos.y = _ScreenPos.y * m_Size.y / WinScale.y;
	ConPos += Trans()->WPOS();
	return ConPos;
}


void GameCam::UpdateNext()
{
	// trans���� ������Ʈ �ǰ� ���� �ϰ� �ϱ� ���ؼ� ���⼭ �ߴ�.
	m_MatView.VIEWTOLH(Trans()->LPOS(), Trans()->WFORWARD(), Trans()->WUP());

	switch (m_Type)
	{
	case GameCam::PERS:
		m_MatProj.PERSLHDEG(m_Fov, m_Size.x , m_Size.y , m_Near, m_Far);
		break;
	case GameCam::ORTH:
		m_MatProj.ORTHLH(m_Size, m_Near, m_Far);
		break;
	default:
		break;
	}
	
}