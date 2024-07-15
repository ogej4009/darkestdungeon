#include "GameFreeCam.h"
#include <GameInput.h>
#include <GameWin.h>

GameFreeCam::GameFreeCam()
{

}

GameFreeCam::~GameFreeCam()
{

}

void GameFreeCam::Init()
{
	{
		if (false == GameInput::IsKey(L"LMOVE")) { GameInput::CreateKey(L"LMOVE", 'A'); }
		if (false == GameInput::IsKey(L"RMOVE")) { GameInput::CreateKey(L"RMOVE", 'D'); }
		if (false == GameInput::IsKey(L"UMOVE")) { GameInput::CreateKey(L"UMOVE", 'Q'); }
		if (false == GameInput::IsKey(L"DMOVE")) { GameInput::CreateKey(L"DMOVE", 'E'); }
		if (false == GameInput::IsKey(L"FMOVE")) { GameInput::CreateKey(L"FMOVE", 'W'); }
		if (false == GameInput::IsKey(L"BMOVE")) { GameInput::CreateKey(L"BMOVE", 'S'); }
		if (false == GameInput::IsKey(L"ZROT+")) { GameInput::CreateKey(L"ZROT+", 'Z'); }
		if (false == GameInput::IsKey(L"ZROT-")) { GameInput::CreateKey(L"ZROT-", 'X'); }
		if (false == GameInput::IsKey(L"XROT+")) { GameInput::CreateKey(L"XROT+", 'Z'); }
		if (false == GameInput::IsKey(L"XROT-")) { GameInput::CreateKey(L"XROT-", 'X'); }
		if (false == GameInput::IsKey(L"YROT+")) { GameInput::CreateKey(L"YROT+", 'Z'); }
		if (false == GameInput::IsKey(L"YROT-")) { GameInput::CreateKey(L"YROT-", 'X'); }
		if (false == GameInput::IsKey(L"CAMMOVE")) { GameInput::CreateKey(L"CAMMOVE", VK_RBUTTON); }
	}
	//유니티식 프리카메라를 만들 생각이다.
	//마우스 오른쪽 클릭 + 이동
}

void GameFreeCam::Update()
{
	if (true == GameInput::Press(L"CAMMOVE"))
	{
		if (GameWin::MainWindow()->MouseDir3D() != CVector::ZERO)
		{
			// X를 돌리는게 
			Trans()->LROTY(GameWin::MainWindow()->MouseDir3D().x * GameTime::DeltaTimeToSpeed(180.0f));
			Trans()->LROTX(-(GameWin::MainWindow()->MouseDir3D().y) * GameTime::DeltaTimeToSpeed(180.0f));
		}


		if (true == GameInput::Press(L"LMOVE")) {
			Trans()->LMOVE(Trans()->WLEFT() * GameTime::DeltaTime());
		}
		if (true == GameInput::Press(L"RMOVE")) {
			Trans()->LMOVE(Trans()->WRIGHT() * GameTime::DeltaTime());
		}
		if (true == GameInput::Press(L"UMOVE")) {
			Trans()->LMOVE(Trans()->WUP() * GameTime::DeltaTime());
		}
		if (true == GameInput::Press(L"DMOVE")) {
			Trans()->LMOVE(Trans()->WDOWN() * GameTime::DeltaTime());
		}
		if (true == GameInput::Press(L"FMOVE")) {
			Trans()->LMOVE(Trans()->WFORWARD() * GameTime::DeltaTime());
		}
		if (true == GameInput::Press(L"BMOVE")) {
			Trans()->LMOVE(Trans()->WBACK() * GameTime::DeltaTime());
		}
	}
}

void GameFreeCam::DebugRender()
{

}