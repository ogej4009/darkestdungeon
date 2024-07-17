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

	//����� ��Ʈ ó����
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"DEBUG3D");
		RD->VTXShader(L"DebugShader.fx");
		RD->PIXShader(L"DebugShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}

	//�׸��� �������� �����н�
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"GRID");
		RD->VTXShader(L"GridShader.fx");
		RD->PIXShader(L"GridShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}
}