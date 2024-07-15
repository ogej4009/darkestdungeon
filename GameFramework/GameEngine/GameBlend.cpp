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
	// output merger�� ��������
	// ������ �ް�, ����° �Ű������� ���ÿɼ����� ��Ʈ����ũ���� ��
	// üũ�� �κ��� ������, 4����Ʈ ũ���, 32��Ʈ
	GameDevice::MainContext()->OMSetBlendState(m_pBlend, nullptr, 0xffffffff);
}