#pragma once

#include <GameRes.h>
#include "GameDevice.h"

class GameDepthStencil : public GameRes<GameDepthStencil>
{
private:
	D3D11_DEPTH_STENCIL_DESC m_Decs;
	ID3D11DepthStencilState* m_pDSS;

public:
	static SPtr<GameDepthStencil> Create(const GameString& _Name, const D3D11_DEPTH_STENCIL_DESC& _Decs)
	{
		GameDepthStencil* NewRes = CreateRes(_Name);
		NewRes->Create(_Decs);
		return NewRes;
	}

public:
	bool Create(const D3D11_DEPTH_STENCIL_DESC& _Decs);
	void Setting();

public:
	GameDepthStencil();
	~GameDepthStencil();
};
