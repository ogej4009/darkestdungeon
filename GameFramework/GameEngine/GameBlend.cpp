#include "GameBlend.h"



GameBlend::GameBlend()
{
}


GameBlend::~GameBlend()
{
	if (nullptr != m_pBlend)
	{
		m_pBlend->Release();
	}
}

bool GameBlend::Create(const D3D11_BLEND_DESC& _Decs)
{
	m_Decs = _Decs;

	if (S_OK != GameDevice::MainDevice()->CreateBlendState(&m_Decs, &m_pBlend))
	{
		ASSERTMSG;
		return false;
	}

	return true;
}

void GameBlend::Setting()
{
	// output merger에 세팅해줌
	// 포인터 받고, 세번째 매개변수는 세팅옵션으로 비트마스크같은 것
	// 체크된 부분을 적용함, 4바이트 크기네, 32비트
	GameDevice::MainContext()->OMSetBlendState(m_pBlend, nullptr, 0xffffffff);
}