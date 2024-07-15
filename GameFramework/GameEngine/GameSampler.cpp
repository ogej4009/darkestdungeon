#include "GameSampler.h"



GameSampler::GameSampler() : m_pSmp(nullptr)
{
}


GameSampler::~GameSampler()
{
	if (nullptr != m_pSmp)
	{
		m_pSmp->Release();
	}
}

bool GameSampler::Create(const D3D11_SAMPLER_DESC& _Decs)
{
	m_Decs = _Decs;

	if (S_OK != GameDevice::MainDevice()->CreateSamplerState(&m_Decs, &m_pSmp))
	{
		ASSERTMSG;
		return false;
	}

	return true;
}

void GameSampler::Setting(GameShader::SHADER_TYPE _Type, int _Index)
{
	switch (_Type)
	{
	case GameShader::SHADER_TYPE::ST_VS:
		VSSetting(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_HS:
		HSSetting(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_DS:
		DSSetting(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_GS:
		GSSetting(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_PS:
		PSSetting(_Index);
		break;
	default:
		break;
	}
}

void GameSampler::VSSetting(int _Index) {
	GameDevice::MainContext()->VSSetSamplers(_Index, 1, &m_pSmp);
}
void GameSampler::HSSetting(int _Index) {
	GameDevice::MainContext()->HSSetSamplers(_Index, 1, &m_pSmp);
}
void GameSampler::DSSetting(int _Index) {
	GameDevice::MainContext()->DSSetSamplers(_Index, 1, &m_pSmp);
}
void GameSampler::GSSetting(int _Index) {
	GameDevice::MainContext()->GSSetSamplers(_Index, 1, &m_pSmp);
}
void GameSampler::PSSetting(int _Index) {
	GameDevice::MainContext()->PSSetSamplers(_Index, 1, &m_pSmp);
}