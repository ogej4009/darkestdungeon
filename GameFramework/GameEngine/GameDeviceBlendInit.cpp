#include "GameDevice.h"
#include "GameBlend.h"


void GameDevice::BlendInit()
{
	{
		D3D11_BLEND_DESC NewDesc;

		// ���̹��۰� ���϶� ���İ����� ������ �ٰ��̳� �ƴҰ��̳ĸ� ����
		NewDesc.AlphaToCoverageEnable = false;
		NewDesc.IndependentBlendEnable = false;
		NewDesc.RenderTarget[0];
		memset(&NewDesc.RenderTarget[0], 0, sizeof(NewDesc.RenderTarget[0]));

		NewDesc.RenderTarget[0].BlendEnable = TRUE;
		//src���� ���� �Ķ� ȭ�� ���� ���� �̹����� �ǹ��Ѵ�.
		//���� ����ũ?
		NewDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		//�� RGB ���� �����ڸ� ����
		NewDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		//RGB���� � ���·� �����ð��� ������ factor�� ����
		NewDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		NewDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		//���ĳ��� ��� �������� ����
		NewDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		//Alpha���� � ���·� �����ð��� ������ factor�� ����
		NewDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		NewDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


		GameBlend::Create(L"DEFBLEND", NewDesc);
	}
}

