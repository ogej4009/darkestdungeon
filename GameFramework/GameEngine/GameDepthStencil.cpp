#include "GameDepthStencil.h"


GameDepthStencil::GameDepthStencil()
{
}


GameDepthStencil::~GameDepthStencil()
{
	if (nullptr != m_pDSS)
	{
		m_pDSS->Release();
	}
}


bool GameDepthStencil::Create(const D3D11_DEPTH_STENCIL_DESC& _Decs)
{
	m_Decs = _Decs;

	if (S_OK != GameDevice::MainDevice()->CreateDepthStencilState(&m_Decs, &m_pDSS))
	{
		ASSERTMSG;
		return false;
	}

	return true;

}

void GameDepthStencil::Setting()
{
	GameDevice::MainContext()->OMSetDepthStencilState(m_pDSS, 0);
}