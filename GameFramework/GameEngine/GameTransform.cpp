#include "GameTransform.h"
#include "GameActor.h"
#include "GameCam.h"

GameTransform::GameTransform() 
	: WorldFunc(&GameTransform::CalWorld)
	, m_LScale(CVector::ONE)
	, m_LRot(CVector::ZERO)
	, m_LPos(CVector::ZERO)
{
}


GameTransform::~GameTransform()
{
}

void GameTransform::Update()
{
	DebugCheck();

	m_MatScale.Scale(m_LScale);

	m_MatRotX.RotXDeg(m_LRot.x);
	m_MatRotY.RotYDeg(m_LRot.y);
	m_MatRotZ.RotZDeg(m_LRot.z);
	m_MatRot = m_MatRotX * m_MatRotY * m_MatRotZ;

	m_MatPos.Pos(m_LPos);
	(this->*WorldFunc)();
	QSetting();
}

void GameTransform::CalWorld()
{
	m_LWorld = m_MatScale * m_MatRot * m_MatPos;
	m_WWorld = m_LWorld;

	m_WRot = m_LRot;
	m_WScale = m_LScale;
	m_WPos = m_WWorld.ArrVec[3];
}

//부모행렬의 월드를 곱해서 따라가게 할 수 있음
void GameTransform::CalParentWorld()
{	
	m_LWorld = m_MatScale * m_MatRot * m_MatPos;
	m_WWorld = m_LWorld * m_pParent->m_WWorld;

	m_WRot = m_pParent->WROT() + m_LRot;
	m_WScale = m_pParent->WSCALE() * m_LScale;
	m_WPos = m_WWorld.ArrVec[3];
}


void GameTransform::Init()
{
	if (nullptr != Actor()->GetCom<GameTransform>())
	{
		ASSERTMSG;
	}
}

void GameTransform::CalTransData(GameCam* _Cam) 
{
	m_TransData.W = m_WWorld;
	m_TransData.V = _Cam->MatView();
	m_TransData.P = _Cam->MatProj();
	m_TransData.WV = m_TransData.W * m_TransData.V;
	m_TransData.VP = _Cam->MatViewProj();
	m_TransData.WVP = m_TransData.W * m_TransData.V * m_TransData.P;

	m_TransData.AllTP();
}

void GameTransform::QSetting()
{
	m_LRotQ = m_LRot;
	m_WRotQ = m_WRot;

	m_LRotQ.EulerToQ();
	m_WRotQ.EulerToQ();
}