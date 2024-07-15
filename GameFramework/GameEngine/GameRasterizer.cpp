#include "GameRasterizer.h"

GameRasterizer::GameRasterizer()
{
}


GameRasterizer::~GameRasterizer()
{
	if (nullptr != m_pRasterizer)
	{
		m_pRasterizer->Release();
	}
}

bool GameRasterizer::Create(const D3D11_RASTERIZER_DESC& _Decs)
{
	m_Decs = _Decs;

	if (S_OK != GameDevice::MainDevice()->CreateRasterizerState(&m_Decs, &m_pRasterizer))
	{
		ASSERTMSG;
		return false;
	}

	return true;
}

void GameRasterizer::Setting()
{
	GameDevice::MainContext()->RSSetState(m_pRasterizer);
}