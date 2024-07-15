#pragma once
#include "GameRenderer.h"

class GameGrid : public GameRenderer
{
public:
	class SGRIDDATA
	{
	public:
		float Step;
		float Border;
		float MinA;
		float MaxA;
		CVector Color;
		CVector CamPos;
	};

public:
	SGRIDDATA m_Data;

private:
	GameRenderData* RD;

public:
	GameGrid();
	~GameGrid();

public:
	void RenderUpdate(SPtr<GameCam> _Cam) override;
	void Init(unsigned int _Order = 0) override;
};

