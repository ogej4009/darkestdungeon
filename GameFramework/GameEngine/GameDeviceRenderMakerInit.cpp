#include "GameDevice.h"
#include "GameRenderMaker.h"

void GameDevice::RenderPathInit()
{
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"2DSPRITE");
		RD->VTXShader(L"TexShader.fx");
		RD->PIXShader(L"TexShader.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}

	//����� ��Ʈ ó����
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"DEBUG3D");
		RD->VTXShader(L"DEBUGSH.fx");
		RD->PIXShader(L"DEBUGSH.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}

	//�׸��� �������� �����н�
	{
		SPtr<GameRenderMaker> RD = GameRenderMaker::Create(L"GRID");
		RD->VTXShader(L"GRID.fx");
		RD->PIXShader(L"GRID.fx");
		RD->Blend(L"DEFBLEND");
		RD->Rasterizer(L"2DNONE");
		RD->DepthStencil(L"DEFDEPTH");
	}
}