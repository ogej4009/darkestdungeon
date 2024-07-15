#include "GameDevice.h"
#include "GameBlend.h"


void GameDevice::BlendInit()
{
	{
		D3D11_BLEND_DESC NewDesc;

		// 깊이버퍼가 쓰일때 알파값으로 영향을 줄것이냐 아닐것이냐를 결정
		NewDesc.AlphaToCoverageEnable = false;
		NewDesc.IndependentBlendEnable = false;
		NewDesc.RenderTarget[0];
		memset(&NewDesc.RenderTarget[0], 0, sizeof(NewDesc.RenderTarget[0]));

		NewDesc.RenderTarget[0].BlendEnable = TRUE;
		//src쪽이 기존 파란 화면 위에 쏴줄 이미지를 의미한다.
		//쓰기 마스크?
		NewDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		//두 RGB 값의 연산자를 선택
		NewDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		//RGB값을 어떤 형태로 가져올건지 곱해줄 factor를 결정
		NewDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		NewDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		//알파끼리 어떻게 연산할지 결정
		NewDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		//Alpha값을 어떤 형태로 가져올건지 곱해줄 factor를 결정
		NewDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		NewDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


		GameBlend::Create(L"DEFBLEND", NewDesc);
	}
}

