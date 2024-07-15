#include "GameDevice.h"
#include "GameSampler.h"



void GameDevice::SmpInit()
{
	{
		// 0.0 0.0       // 2.0 0.0
		// 0.0 1.0       // 2.0 2.0

		D3D11_SAMPLER_DESC SmpData;

		SmpData.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		// �ݺ�
		// D3D11_TEXTURE_ADDRESS_CLAMP // �ڸ��°�.
		// D3D11_TEXTURE_ADDRESS_MIRROR 
		// D3D11_TEXTURE_ADDRESS_BORDER

		SmpData.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpData.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpData.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		// ����
		// SmpData.BorderColor
		SmpData.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//D3D11_FILTER_ANISOTROPIC; ����϶� �ǹ̰� �������ε�. �Ⱦ���. 
		SmpData.MaxAnisotropy = 0;
		SmpData.MinLOD = -FLT_MAX;
		SmpData.MaxLOD = FLT_MAX;
		SmpData.MipLODBias = 1.0f;

		GameSampler::Create(L"LSMP", SmpData);
	}

	{

		D3D11_SAMPLER_DESC SmpData;

		SmpData.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

		// �ݺ�

		// D3D11_TEXTURE_ADDRESS_CLAMP // �ڸ��°�.
		// D3D11_TEXTURE_ADDRESS_MIRROR 
		// D3D11_TEXTURE_ADDRESS_BORDER

		SmpData.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpData.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpData.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		// ����
		// SmpData.BorderColor
		SmpData.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//D3D11_FILTER_ANISOTROPIC; ����϶� �ǹ̰� �������ε�. �Ⱦ���. 
		SmpData.MaxAnisotropy = 0;
		SmpData.MinLOD = -FLT_MAX;
		SmpData.MaxLOD = FLT_MAX;
		SmpData.MipLODBias = 1.0f;

		GameSampler::Create(L"PSMP", SmpData);
	}
}
