#pragma once
#include "GameDevice.h"
#include <GameRes.h>

class GameBlend : public GameRes<GameBlend>
{
private:
	D3D11_BLEND_DESC m_Decs;
	ID3D11BlendState* m_pBlend;

public:
	GameBlend();
	~GameBlend();

public:
	static SPtr<GameBlend> Create(const GameString& _Name, const D3D11_BLEND_DESC& _Decs)
	{
		GameBlend* NewRes = CreateRes(_Name);
		NewRes->Create(_Decs);
		return NewRes;
	}

	bool Create(const D3D11_BLEND_DESC& _Decs);
	void Setting();
};

