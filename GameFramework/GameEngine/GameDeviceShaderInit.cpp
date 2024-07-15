#include "GameDevice.h"
#include "GameVtxHeader.h"
#include <GameDic.h>
#include <vector>
#include "GameVtxShader.h"
#include "GamePixShader.h"
#include "GameGrid.h"

void GameDevice::ShaderInit()
{
	// �ϴ� ��θ� ã�ƿ���
	GameDic m_Dir;
	//�ش� �̸��� ������ �ִ� �θ���� �̵��ϱ�
	m_Dir.MoveParent(L"SCJ_DX");
	//�ش� �̸��� ������ �ִ� �ڽ��� ã�Ƽ� �̵�, ���н� false ����
	if (false == m_Dir.TryFindFileRecursive(L"SHADER"))
	{
		ASSERTMSG;
	}

	{
		SPtr<GameVtxShader> VTXPTR = GameVtxShader::Load(m_Dir.CreateGameFilePath(L"TexShader.fx"), L"VS_TEX", 5, 0);
		VTXPTR->LayOut.AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTXPTR->LayOut.AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTXPTR->LayOut.EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

		VTXPTR->CB<CTransData>(L"CTransData", 0);
		VTXPTR->CB<CVector>(L"TEXCUT", 1);

		SPtr<GamePixShader> PIXPTR = GamePixShader::Load(m_Dir.CreateGameFilePath(L"TexShader.fx"), L"PS_TEX", 5, 0);
		//PIXPTR->CB<CMatrix>(L"LaplaceFilter",0);
		PIXPTR->CB<CVector>(L"TEXCOLOR", 0);
		PIXPTR->TEX(L"SpriteTex", 0);
		PIXPTR->SMP(L"LSMP", 0);
		PIXPTR->SMP(L"PSMP", 1);

	}

	//����� ó���� ���̴�
	{
		SPtr<GameVtxShader> VTXPTR = GameVtxShader::Load(m_Dir.CreateGameFilePath(L"DEBUGSH.fx"), L"VS_DEBUG", 5, 0);
		VTXPTR->LayOut.EndLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTXPTR->CB<CTransData>(L"CTransData", 0);

		SPtr<GamePixShader> PIXPTR = GamePixShader::Load(m_Dir.CreateGameFilePath(L"DEBUGSH.fx"), L"PS_DEBUG", 5, 0);
		PIXPTR->CB<CVector>(L"DEBUGCOLOR", 0);
	}

	//�׸��� ������ ���̴�
	{
		SPtr<GameVtxShader> VTXPTR = GameVtxShader::Load(m_Dir.CreateGameFilePath(L"GRID.fx"), L"VS_GRID", 5, 0);
		VTXPTR->LayOut.EndLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		VTXPTR->CB<CTransData>(L"CTransData", 0);

		SPtr<GamePixShader> PIXPTR = GamePixShader::Load(m_Dir.CreateGameFilePath(L"GRID.fx"), L"PS_GRID", 5, 0);
		PIXPTR->CB<GameGrid::SGRIDDATA>(L"GRIDDATA", 1);
	}
}