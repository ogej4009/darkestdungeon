#pragma once
#include "GameLogic.h"

// �������� �����ϴ� �������
// � ī�޶� �־��ָ� �׳༮�� 3�������� FREE�ϰ� �����ϼ� �ִ�.

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

