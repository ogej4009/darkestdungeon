#pragma once
#include <Windows.h>
class SoftRenderFrame
{

public:
	SoftRenderFrame();
	~SoftRenderFrame();
	static int testStatic;
// 클래스 내부의 static함수는 __stdcall
public:
	static void WINAPI Progress();
	static void WINAPI Init();
	static void WINAPI LogicUpdate();

};

