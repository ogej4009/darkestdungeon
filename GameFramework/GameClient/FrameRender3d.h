#pragma once
#include <Windows.h>
class FrameRender3d
{
private:
	static float m_FadeTimer;

public:
	static void WINAPI Init();
	static void WINAPI Progress();

public:
	FrameRender3d();
	~FrameRender3d();
};

