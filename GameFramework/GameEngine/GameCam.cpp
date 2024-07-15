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

	//위치정보는 꼭 있어야함, 카메라니까
	if (nullptr == Actor()->GetCom<GameTransform>())
	{
		ASSERTMSG;
	}

	//Actor마다 Cam은 하나씩만 들고있는다.
	if (nullptr != Actor()->GetCom<GameCam>())
	{
		ASSERTMSG;
	}

	//등장할 씬이 없는것도 안되
	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	Actor()->Scene()->RENDERMGR.PushCam(this);
}

CVector GameCam::ScreenPos3DToWorldPos(const CVector& _ScreenPos)
{
	// m_Size 윈도우의 사이즈가 필요하다.
	// m_Size 내가 직교로 화면을 보는 사이즈

	CVector WinScale = GameWin::MainWindow()->Size();
	// 왼쪽위 점이 0,0
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
	// trans폼이 업데이트 되고 나서 하게 하기 위해서 여기서 했다.
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