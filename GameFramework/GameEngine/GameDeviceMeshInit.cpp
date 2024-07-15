#include "GameDevice.h"
#include "GameVtxBuffer.h"
#include "GameIdxBuffer.h"
#include "GameVtxHeader.h"
#include "GameMesh.h"
#include <GameMath.h>
#include <vector>

void GameDevice::MeshInit()
{
#pragma region 2DRECT
	{
		// 소프트웨어 랜더링에서 로컬 포스 만드는 작업하고 있다.

		// int
		// 서브셋
		// 0 1 2 12
		// 0 2 3 12
		// 24 바이트
		// 48바이트짜리 점을 4개
		// 48 * 4 + 4 * 6 ? 48 * 6 

		// 48바이트짜리 점을 6개
		// 48 * 6 

		std::vector<Vertex2d> ArrVTX;
		ArrVTX.resize(4);

		ArrVTX[0].m_Pos = { -0.5f, 0.5f, 0.0f, 1.0f };
		ArrVTX[1].m_Pos = { 0.5f, 0.5f, 0.0f,  1.0f };
		ArrVTX[2].m_Pos = { 0.5f, -0.5f, 0.0f,  1.0f };
		ArrVTX[3].m_Pos = { -0.5f, -0.5f, 0.0f,  1.0f };

		ArrVTX[0].m_UV = { 0.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[1].m_UV = { 1.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[2].m_UV = { 1.0f, 1.0f, 0.0f, 0.0f };
		ArrVTX[3].m_UV = { 0.0f, 1.0f, 0.0f, 0.0f };

		ArrVTX[0].m_Color = CVector::WHITE;
		ArrVTX[1].m_Color = CVector::WHITE;
		ArrVTX[2].m_Color = CVector::WHITE;
		ArrVTX[3].m_Color = CVector::WHITE;

		GameVtxBuffer::Create(L"2DRECT", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		std::vector<UINT> ArrIDX = { 0, 1, 2, 0, 2, 3 };

		GameIdxBuffer::Create(L"2DRECT", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);

		GameMesh::Create(L"2DRECT", L"2DRECT", L"2DRECT");

	}
#pragma endregion

#pragma region GRID
	{
		// 소프트웨어 랜더링에서 로컬 포스 만드는 작업하고 있다.
		std::vector<CVector> ArrVTX;
		ArrVTX.resize(4);

		ArrVTX[0] = { -0.5f, 0.0f, -0.5f, 1.0f };
		ArrVTX[1] = { 0.5f, 0.0f, -0.5f, 1.0f };
		ArrVTX[2] = { 0.5f, 0.0f, 0.5f, 1.0f };
		ArrVTX[3] = { -0.5f, 0.0f, 0.5f, 1.0f };

		GameVtxBuffer::Create(L"GRID", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0, 1, 2, 0, 2, 3 };

		GameIdxBuffer::Create(L"GRID", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"GRID", L"GRID", L"GRID");

	}
#pragma endregion

#pragma region MainRect
	{
		std::vector<Vertex2d> ArrVTX;
		ArrVTX.resize(4);


		ArrVTX[0].m_Pos = { -0.48f, 0.26f, 0.0f, 1.0f };
		ArrVTX[1].m_Pos = { 0.48f, 0.26f, 0.0f,  1.0f };
		ArrVTX[2].m_Pos = { 0.48f, -0.26f, 0.0f,  1.0f };
		ArrVTX[3].m_Pos = { -0.48f, -0.26f, 0.0f,  1.0f };

		ArrVTX[0].m_UV = { 0.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[1].m_UV = { 1.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[2].m_UV = { 1.0f, 1.0f, 0.0f, 0.0f };
		ArrVTX[3].m_UV = { 0.0f, 1.0f, 0.0f, 0.0f };

		ArrVTX[0].m_Color = CVector::WHITE;
		ArrVTX[1].m_Color = CVector::WHITE;
		ArrVTX[2].m_Color = CVector::WHITE;
		ArrVTX[3].m_Color = CVector::WHITE;

		GameVtxBuffer::Create(L"MainRect", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0, 1, 2, 0, 2, 3 };

		GameIdxBuffer::Create(L"MainRect", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"MainRect", L"MainRect", L"MainRect");

	}
#pragma endregion


#pragma region ScreenRect
	{
		std::vector<Vertex2d> ArrVTX;
		ArrVTX.resize(4);


		ArrVTX[0].m_Pos = { -0.465f, 0.262f, 0.0f, 1.0f };
		ArrVTX[1].m_Pos = { 0.465f, 0.262f, 0.0f,  1.0f };
		ArrVTX[2].m_Pos = { 0.465f, -0.262f, 0.0f,  1.0f };
		ArrVTX[3].m_Pos = { -0.465f, -0.262f, 0.0f,  1.0f };

		ArrVTX[0].m_UV = { 0.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[1].m_UV = { 1.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[2].m_UV = { 1.0f, 1.0f, 0.0f, 0.0f };
		ArrVTX[3].m_UV = { 0.0f, 1.0f, 0.0f, 0.0f };

		ArrVTX[0].m_Color = CVector::WHITE;
		ArrVTX[1].m_Color = CVector::WHITE;
		ArrVTX[2].m_Color = CVector::WHITE;
		ArrVTX[3].m_Color = CVector::WHITE;

		GameVtxBuffer::Create(L"ScreenRect", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0, 1, 2, 0, 2, 3 };

		GameIdxBuffer::Create(L"ScreenRect", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"ScreenRect", L"ScreenRect", L"ScreenRect");

	}
#pragma endregion


#pragma region FullRect
	{
		std::vector<Vertex2d> ArrVTX;
		ArrVTX.resize(4);


		ArrVTX[0].m_Pos = { 2.0f, 1.0f, -0.5f, 1.0f };
		ArrVTX[1].m_Pos = { 3.0f, 1.0f, -0.5f,  1.0f };
		ArrVTX[2].m_Pos = { 3.0f, -1.0f, -0.5f,  1.0f };
		ArrVTX[3].m_Pos = { 2.0f, -1.0f, -0.5f,  1.0f };

		ArrVTX[0].m_UV = { 0.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[1].m_UV = { 1.0f, 0.0f, 0.0f, 0.0f };
		ArrVTX[2].m_UV = { 1.0f, 1.0f, 0.0f, 0.0f };
		ArrVTX[3].m_UV = { 0.0f, 1.0f, 0.0f, 0.0f };

		ArrVTX[0].m_Color = CVector::WHITE;
		ArrVTX[1].m_Color = CVector::WHITE;
		ArrVTX[2].m_Color = CVector::WHITE;
		ArrVTX[3].m_Color = CVector::WHITE;

		GameVtxBuffer::Create(L"FullRect", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0, 1, 2, 0, 2, 3 };

		GameIdxBuffer::Create(L"FullRect", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"FullRect", L"FullRect", L"FullRect");

	}
#pragma endregion

#pragma region DEBUGRECT	
	{
		std::vector<CVector> ArrVTX;

		ArrVTX.resize(36);

		ArrVTX[0] = { -0.5f, 0.5f, 0.0f, 1.0f };
		ArrVTX[1] = { 0.5f, 0.5f, 0.0f, 1.0f };
		ArrVTX[2] = { 0.5f, -0.5f, 0.0f, 1.0f };
		ArrVTX[3] = { -0.5f, -0.5f, 0.0f, 1.0f };

		GameVtxBuffer::Create(L"DEBUG2DRECT", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0,1,2,3,0 };
		GameIdxBuffer::Create(L"DEBUG2DRECT", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"DEBUG2DRECT", L"DEBUG2DRECT", L"DEBUG2DRECT");
	}
#pragma endregion

#pragma region Circle	
	{
		std::vector<CVector> ArrVTX;

		ArrVTX.resize(13);

		float theta = 30.0f;
		CVector startPos = { 0.0f, 1.0f,0.0f,1.0f };
		CMatrix RotateMat;
		RotateMat.IDEN();

		ArrVTX[0] = startPos;
		for (int i = 0; i < 13; ++i)
		{
			RotateMat.ArrVec[0] = { cosf(GameMath::DTOR * theta * i), sinf(GameMath::DTOR * theta * i),0.0f,1.0f };
			RotateMat.ArrVec[1] = { sinf(GameMath::DTOR * theta * i), cosf(GameMath::DTOR * theta * i) ,0.0f,1.0f };
			ArrVTX[i] = startPos * RotateMat;
		}
		GameVtxBuffer::Create(L"Circle", ArrVTX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		std::vector<UINT> ArrIDX = { 0,1,2,3,4,5,6,7,8,9,10,11,12,0 };
		GameIdxBuffer::Create(L"Circle", ArrIDX, D3D11_USAGE::D3D11_USAGE_DEFAULT);
		GameMesh::Create(L"Circle", L"Circle", L"Circle");

	
	}
#pragma endregion
}