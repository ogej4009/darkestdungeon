#include <Windows.h>
#include <GameWin.h>
#include "SoftRenderFrame.h"
#include "FrameRender3d.h"
#include <GameDevice.h>
#include <GameDebug.h>
#include <GameString.h>
#include <GameDic.h>
#include <LogicHeader.h>

// SPINE
#pragma comment(lib, "GameBase.lib")
#pragma comment(lib, "GameEngine.lib")
#pragma comment(lib, "GameLib.lib")

void MakeAtlas();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 연산자 오버로딩 기억나시나요???
	// 그중 종류가 2개다.
	// 전역 연산자 겹지정
	// 맴버 연산자 겹지정


	GameDebug::LeakCheckSetting();

	int* test = new int();

	// 메모리
	// 레퍼런스 카운팅
	// 일반적인 매니지드 언어에서 사용하는 방식이다.
	// GC라고 하는애가 레퍼런스 카운트라는 것을 기반으로해서
	// 지워줘야 할때 지워준다. 

	CVector Vec1 = { 1.0f, 0.0f, 0.0f };
	CVector Vec2 = { -1.0f, 0.0f, 1.0f };

	float Test = Vec1.VectorAngleDeg(Vec2);

	SPtr<GameWin> ptrWin = GameWin::CreateSGameWin(L"GAMEWIN");
	//디버그 체크모드를 넣을 수 있음
	ptrWin->DebugOn();
	ptrWin->Size(LogicHeader::WindowSize);
	
	//MakeAtlas();


	//SoftRenderFrame
	//FrameRender3d
	GameDevice::CreateDevice(ptrWin);
	GameWin::BaseLoop(FrameRender3d::Progress, FrameRender3d::Init);
}

void MakeAtlas()
{
	{
		GameDic ChangeExtDir;
		ChangeExtDir.PathSetting(L"C:\\Users\\chanjong\\Documents\\Learn_Tutorial\\DarkestSpine\\Assets\\MakeAnim\\monster\\Common\\brigand_fusilier\\anim");
		std::vector<GameFile> List = ChangeExtDir.DirToAllFile();
		for (auto& item : List)
		{
			item.AddExt(L".skel", ".bytes");
			item.AddExt(L".atlas", ".txt");
		}
	}

	{
		GameDic ChangeExtDir;
		ChangeExtDir.PathSetting(L"C:\\Users\\chanjong\\Documents\\Learn_Tutorial\\DarkestSpine\\Assets\\MakeAnim\\monster\\Common\\brigand_fusilier\\fx");
		std::vector<GameFile> List = ChangeExtDir.DirToAllFile();
		for (auto& item : List)
		{
			item.AddExt(L".skel", ".bytes");
			item.AddExt(L".atlas", ".txt");
		}
	}
	int a = 0;
}