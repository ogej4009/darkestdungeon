#pragma once
#include <Windows.h>
class SoftRenderFrame
{

public:
	SoftRenderFrame();
	~SoftRenderFrame();
	static int testStatic;
// Ŭ���� ������ static�Լ��� __stdcall
public:
	static void WINAPI Progress();
	static void WINAPI Init();
	static void WINAPI LogicUpdate();

};

