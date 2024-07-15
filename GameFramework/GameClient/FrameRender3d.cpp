#include "S3DRENDERFRAME.h"
#include <SGAMEWIN.h>
#include <SGAMEDEVICE.h>
#include <SMESH.h>
#include <SVTXSHADER.h>
#include <SPIXSHADER.h>
#include <SGAMEINPUT.h>
#include <SGAMETIME.h>
#include <SGAMEDIR.h>
#include <STEX.h>
#include <SSMP.h>
#include <SBLEND.h>
#include <SGAMESCENE.h>

#include "STITLECOM.h"
#include "SDUNGEONCOM.h"
#include "S3DTITLECOM.h"

FrameRender3d::FrameRender3d()
{
}


FrameRender3d::~FrameRender3d()
{
}


void FrameRender3d::Init()
{
	GameScene::CreateScene<TitleScene>("TITLE");
	//GameScene::ChangeScene("TITLE");

	//GameScene::CreateScene<STITLECOM/*, HTITLECOM, HTITLECOM*/>("TITLE");
	/*SPtr<GameScene> PlayScene = */GameScene::CreateScene<DungeonScene>("PLAY");
	GameScene::CreateScene("ENDING");
	GameScene::ChangeScene("PLAY");
}

void FrameRender3d::Progress()
{
	GameTime::Update();
	GameInput::Update();
	GameScene::Progress();
	GameWin::AllWinUpdate();
}

