#pragma once
#include "GameDevice.h"
#include <GameRes.h>

//메쉬와 관련하여 어떤 형식으로 그릴지를 결정하는것
//디폴트는 시계방향만 그려라
//옵션은 시계,반시계,둘다
class GameRasterizer : public GameRes<GameRasterizer>
{
private: 
	D3D11_RASTERIZER_DESC m_Decs;
	ID3D11RasterizerState* m_pRasterizer;

public:
	GameRasterizer();
	~GameRasterizer();

public:
	static SPtr<GameRasterizer> Create(const GameString& _Name, const D3D11_RASTERIZER_DESC& _Decs)
	{
		GameRasterizer* NewRes = CreateRes(_Name);
		NewRes->Create(_Decs);
		return NewRes;
	}

	bool Create(const D3D11_RASTERIZER_DESC& _Decs);
	void Setting();
};

