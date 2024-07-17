#include "GameDevice.h"
#include "GameRenderMaker.h"

void GameDevice::RenderPathInit()
{
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"2DSPRITE");
		RD->VTXShader(L"SpriteShader.fx");
		RD->PIXShader(L"SpriteShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}

	//디버그 랙트 처리용
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"DEBUG3D");
		RD->VTXShader(L"DebugShader.fx");
		RD->PIXShader(L"DebugShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}

	//그리드 랜더러용 랜더패스
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"GRID");
		RD->VTXShader(L"GridShader.fx");
		RD->PIXShader(L"GridShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}
}