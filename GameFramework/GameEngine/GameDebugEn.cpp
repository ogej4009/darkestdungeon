#include "GameDebugEn.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameCam.h"
#include "GameFont.h"

size_t GameDebugEn::m_DrawCount = 0;
size_t GameDebugEn::m_DrawFontCount = 0;

CTransData GameDebugEn::m_DebugTransData;
SPtr<GameRenderData> GameDebugEn::RectRenderData;

std::vector<GameDebugEn::DEBUGDRAWINFO> GameDebugEn::m_Info;
std::vector<GameDebugEn::DEBUGFONTINFO> GameDebugEn::m_FontInfo;

SPtr<GameFont> GameDebugEn::m_Font = nullptr;
CVector GameDebugEn::m_DebugColor = CVector::BLUE;
std::list<GameString>::iterator GameDebugEn::m_DebugFontIter;

GameDebugEn::GameDebugEn()
{

}

GameDebugEn::~GameDebugEn()
{

}

void GameDebugEn::Init()
{
	m_DrawCount = 0;
	m_Info.resize(100);
	m_FontInfo.resize(100);

	RectRenderData = new GameRenderData(L"Circle", "DEBUG3D");
	RectRenderData->m_pData = &m_DebugTransData;
	RectRenderData->RenderPathInit();
	RectRenderData->m_DrawMode = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	RectRenderData->CB("DEBUGCOLOR", m_DebugColor, true);

	m_DebugFontIter = GameFont::m_FontList.begin();
}

void GameDebugEn::DebugRender()
{
	for (size_t i = 0; i < m_DrawCount; i++)
	{
		m_DebugTransData.W = m_Info[i].m_Trans->CREFWW();
		//z-order 정리를 위해서
		m_DebugTransData.W.ArrVec[3].z -= 0.1f;
		m_DebugTransData.V = m_Info[i].m_Cam->MatView();
		m_DebugTransData.P = m_Info[i].m_Cam->MatProj();
		m_DebugTransData.WV = m_DebugTransData.W * m_DebugTransData.V;
		m_DebugTransData.VP = m_Info[i].m_Cam->MatViewProj();
		m_DebugTransData.WVP = m_DebugTransData.W * m_DebugTransData.V * m_DebugTransData.P;
		m_DebugTransData.AllTP();
		m_DebugColor = m_Info[i].Color;

		m_Info[i].RD->Render();
	}

	m_DrawCount = 0;

	
	if (m_DebugFontIter == GameFont::m_FontList.end())
	{
		ASSERTMSG;
	}

	m_Font = GameFont::Find(*m_DebugFontIter);

	if (nullptr == m_Font)
	{
		ASSERTMSG;
	}

	for (size_t i = 0; i < m_DrawFontCount; i++)
	{
		m_Font->DrawUI(m_FontInfo[i]._Text, m_FontInfo[i]._Size, m_FontInfo[i]._Pos, m_FontInfo[i]._Color);
	}
	m_DrawFontCount = 0;

	GameDevice::MainContext()->VSSetShader(nullptr, nullptr, 0);
	GameDevice::MainContext()->PSSetShader(nullptr, nullptr, 0);
	GameDevice::MainContext()->DSSetShader(nullptr, nullptr, 0);
	GameDevice::MainContext()->HSSetShader(nullptr, nullptr, 0);
	GameDevice::MainContext()->GSSetShader(nullptr, nullptr, 0);
}

void GameDebugEn::Draw2DRect(GameTransform* _Trans, CVector _Color)
{
	Draw2DRect(GameScene::CurScene()->RENDERMGR.MainCam(), _Trans, _Color);
}

void GameDebugEn::Draw2DRect(GameCam* _Cam, GameTransform* _Trans, CVector _Color)
{
	m_Info[m_DrawCount].m_Trans = _Trans;
	m_Info[m_DrawCount].m_Cam = _Cam;
	m_Info[m_DrawCount].Color = _Color;
	m_Info[m_DrawCount].RD = RectRenderData;
	++m_DrawCount;
}

void GameDebugEn::Draw2DFont(const GameString& _Text, const float& _Size, const CVector& _Pos, const CVector& _Color)
{
	m_FontInfo[m_DrawFontCount]._Text = _Text;
	m_FontInfo[m_DrawFontCount]._Pos = _Pos;
	m_FontInfo[m_DrawFontCount]._Color = _Color;
	m_FontInfo[m_DrawFontCount]._Size = _Size;
	++m_DrawFontCount;
}