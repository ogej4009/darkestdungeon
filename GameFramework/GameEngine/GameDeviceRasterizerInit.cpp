#include "GameDevice.h"
#include "GameRasterizer.h"



void GameDevice::RasterizerInit()
{
	{
		D3D11_RASTERIZER_DESC m_Decs;
		//���� �����ִ� �ɼǰ��� 0���� �ʱ�ȭ�� ����� ������ �Ȼ����.
		memset(&m_Decs, 0, sizeof(D3D11_RASTERIZER_DESC));

		//���̾�������(�ܰ����� �׷�), �ָ��� ������ �ɼ�
		m_Decs.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		//�ð�,�ݽð�,����� �ɼ�
		m_Decs.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		//��Ƽ���ø� �ɼ�
		m_Decs.MultisampleEnable = FALSE;
		m_Decs.AntialiasedLineEnable = FALSE;

		GameRasterizer::Create(L"2DNONE", m_Decs);
	}
}
