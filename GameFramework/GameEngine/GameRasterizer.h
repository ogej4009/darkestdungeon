#pragma once
#include "GameDevice.h"
#include <GameRes.h>

//�޽��� �����Ͽ� � �������� �׸����� �����ϴ°�
//����Ʈ�� �ð���⸸ �׷���
//�ɼ��� �ð�,�ݽð�,�Ѵ�
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

