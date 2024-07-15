#include "GameDevice.h"
#include "GameRasterizer.h"



void GameDevice::RasterizerInit()
{
	{
		D3D11_RASTERIZER_DESC m_Decs;
		//세팅 안해주는 옵션값은 0으로 초기화를 해줘야 문제가 안생긴다.
		memset(&m_Decs, 0, sizeof(D3D11_RASTERIZER_DESC));

		//와이어프레임(외곽선만 그려), 솔리드 프레임 옵션
		m_Decs.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		//시계,반시계,양방향 옵션
		m_Decs.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		//멀티샘플링 옵션
		m_Decs.MultisampleEnable = FALSE;
		m_Decs.AntialiasedLineEnable = FALSE;

		GameRasterizer::Create(L"2DNONE", m_Decs);
	}
}
