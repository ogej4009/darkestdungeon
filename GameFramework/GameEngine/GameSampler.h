#pragma once
#include "GameDevice.h"
#include "GameShader.h"
#include <GameRes.h>

class GameSampler : public GameRes<GameSampler>
{
private:
	D3D11_SAMPLER_DESC m_Decs;
	ID3D11SamplerState* m_pSmp;

public:
	GameSampler();
	~GameSampler();

public:
	static SPtr<GameSampler> Create(const GameString& _Name, const D3D11_SAMPLER_DESC& _Decs)
	{
		GameSampler* NewRes = CreateRes(_Name);
		NewRes->Create(_Decs);
		return NewRes;
	}

public:
	bool Create(const D3D11_SAMPLER_DESC& _Decs);
	void Setting(GameShader::SHADER_TYPE _Type, int _Index);

private:
	void VSSetting(int _Index);
	void HSSetting(int _Index);
	void DSSetting(int _Index);
	void GSSetting(int _Index);
	void PSSetting(int _Index);
};

