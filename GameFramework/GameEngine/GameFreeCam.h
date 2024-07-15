#pragma once
#include "GameLogic.h"

// 엔진에서 지원하는 기능으로
// 어떤 카메라에 넣어주면 그녀석을 3차원에서 FREE하게 움직일수 있다.

class GameFreeCam : public GameLogic
{
public:
	void Init() override;
	void Update() override;
	void DebugRender() override;

public:
	GameFreeCam();
	~GameFreeCam();
};

