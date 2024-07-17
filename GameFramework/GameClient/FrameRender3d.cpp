#include "FrameRender3d.h"
#include <GameWin.h>
#include <GameDevice.h>
#include <GameMesh.h>
#include <GameVtxShader.h>
#include <GamePixShader.h>
#include <GameInput.h>
#include <GameTime.h>
#include <GameDic.h>
#include <GameTexture.h>
#include <GameSampler.h>
#include <GameBlend.h>
#include <GameScene.h>

#include "TitleScene.h"
#include "DungeonScene.h"


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

