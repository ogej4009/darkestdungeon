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
	// ������ �����ε� ��ﳪ�ó���???
	// ���� ������ 2����.
	// ���� ������ ������
	// �ɹ� ������ ������


	GameDebug::LeakCheckSetting();

	int* test = new int();

	// �޸�
	// ���۷��� ī����
	// �Ϲ����� �Ŵ����� ���� ����ϴ� ����̴�.
	// GC��� �ϴ¾ְ� ���۷��� ī��Ʈ��� ���� ��������ؼ�
	// ������� �Ҷ� �����ش�. 

	CVector Vec1 = { 1.0f, 0.0f, 0.0f };
	CVector Vec2 = { -1.0f, 0.0f, 1.0f };

	float Test = Vec1.VectorAngleDeg(Vec2);

	SPtr<GameWin> ptrWin = GameWin::CreateSGameWin(L"GAMEWIN");
	//����� üũ��带 ���� �� ����
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