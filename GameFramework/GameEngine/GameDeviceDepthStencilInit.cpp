#include "GameDevice.h"
#include "GameDepthStencil.h"

void GameDevice::DepthStencilInit()
{
	{
		D3D11_DEPTH_STENCIL_DESC m_Decs;
		memset(&m_Decs, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

		m_Decs.DepthEnable = true;
		m_Decs.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		m_Decs.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		m_Decs.StencilEnable = false;

		GameDepthStencil::Create(L"DEFDEPTH", m_Decs);
	}
}
