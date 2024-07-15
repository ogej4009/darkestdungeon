#include "GameMath.h"
#include "GameTime.h"
#include "GameMacro.h"
#include <cmath>


int GameRandom::m_Data[32];
float GameRandom::m_Timer = 0;
//단위벡터



UINT CVector::SX = 4;
UINT CVector::SY = 1;
const CVector CVector::ZERO = { 0.0f, 0.0f , 0.0f, 0.0f };
const CVector CVector::ONE = { 1.0f, 1.0f , 1.0f, 1.0f };


const CVector CVector::RED = { 1.0f, 0.0f , 0.0f, 1.0f };
const CVector CVector::GREEN = { 0.0f, 1.0f , 0.0f, 1.0f };
const CVector CVector::BLUE = { 0.0f, 0.0f , 1.0f, 1.0f };
const CVector CVector::WHITE = { 1.0f, 1.0f , 1.0f, 1.0f };

const CVector CVector::LEFT = { -1.0f, 0.0f , 0.0f, 0.0f };
const CVector CVector::RIGHT = { 1.0f, 0.0f , 0.0f, 0.0f };
const CVector CVector::UP = { 0.0f, 1.0f , 0.0f, 0.0f };
const CVector CVector::DOWN = { 0.0f, -1.0f , 0.0f, 0.0f };
const CVector CVector::FORWARD = { 0.0f, 0.0f , 1.0f, 0.0f };
const CVector CVector::BACK = { 0.0f, 0.0f , -1.0f, 0.0f };

UINT CMatrix::SX = 4;
UINT CMatrix::SY = 4;

const float GameMath::PI = 3.14159265359f;
const float GameMath::DTOR = GameMath::PI / 180.0f;
const float GameMath::RTOD = 180.0f / GameMath::PI;

const CMatrix CMatrix::LaplaceMat3D = CMatrix({ -1.0f,-1.0f,-1.0f,0.0f }, { -1.0f,8.0f,-1.0f,0.0f }, { -1.0f,-1.0f,-1.0f,0.0f }, { 0.0f,0.0f,0.0f,0.0f });
//static 함수

CMatrix::CMatrix(CVector _One, CVector _Two, CVector _Three, CVector _Four)
{
	ArrVec[0] = _One;
	ArrVec[1] = _Two;
	ArrVec[2] = _Three;
	ArrVec[3] = _Four;
}


unsigned int GameMath::FormatByteSize(DXGI_FORMAT _Fmt)
{
	// 어차피 4바이트나 16바이트만 취급함
	switch (_Fmt)
	{
	case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
		return 16;
	case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT:
		return 4;
	default:
		ASSERTMSG;
		break;
	}

	ASSERTMSG;
	return 0;
}



GameMath::GameMath()
{

}


GameMath::~GameMath()
{
}



// 벡터    행렬
// 4   1   4  4     1   4
// AX, AY  BX BY == AX, BY
CVector CVector::operator*(const CMatrix& _MAT)
{
	CVector Return = CVector({0.0f,0.0f,0.0f,0.0f});

	//벡터와 행렬의 곱
	for (int i = 0; i < (int)this->SX; ++i)
	{
		for (int j = 0; j < (int)_MAT.SY; ++j)
		{
			Return.Arr[i] += Arr[j] * _MAT.Arr2D[j][i];
		}
	}

	return Return;
}

CVector CVector::operator*=(const CMatrix& _MAT)
{
	float newArr[4] = { 0,0,0,0 };

	//벡터와 행렬의 곱
	for (int i = 0; i < (int)this->SX; ++i)
	{
		for (int j = 0; j < (int)_MAT.SY; ++j)
		{
			newArr[i] += Arr[j] * _MAT.Arr2D[j][i];
		}
	}

	Arr[0] = newArr[0];
	Arr[1] = newArr[1];
	Arr[2] = newArr[2];
	Arr[3] = newArr[3];

	return *this;
}


CVector CVector::TransCoord(const CMatrix& _MAT)
{
	CVector Return = *this;
	Return.w = 1.0f;
	Return = Return * _MAT;
	return Return;
}

CVector CVector::TransNormal(const CMatrix& _MAT)
{
	CVector Return = *this;
	Return.w = 0.0f;
	Return = Return * _MAT;
	return Return;
}

CMatrix CMatrix::operator*(const CMatrix& _MAT)
{
	CMatrix ReturnMat = *this;
	ReturnMat.IDEN();

	ReturnMat.XMMAT = DirectX::XMMatrixMultiply(XMMAT, _MAT.XMMAT);

	//CMatrix  TransposedMat;

	//for (int i = 0; i < (int)CMatrix::SX; ++i)
	//{
	//	for (int j = 0; j < (int)CMatrix::SY; ++j)
	//	{
	//		TransposedMat.Arr2D[i][j] = _MAT.Arr2D[j][i];
	//	}
	//}

	//float temp = 0;

	//for (int i = 0; i < (int)CMatrix::SX; ++i)
	//{
	//	for (int j = 0; j < (int)CMatrix::SX; ++j)
	//	{
	//		temp = 0;
	//		for (int k = 0; k < (int)CMatrix::SY; ++k)
	//		{
	//			temp += Arr2D[i][k] * TransposedMat.Arr2D[j][k];
	//		}
	//		ReturnMat.Arr2D[i][j] = temp;
	//	}
	//}


/*

	ReturnMat.Arr2D[0][0] = (Arr2D[0][0] * _MAT.Arr2D[0][0]) + (Arr2D[0][1] * _MAT.Arr2D[1][0]) + (Arr2D[0][2] * _MAT.Arr2D[2][0]) + (Arr2D[0][3] * _MAT.Arr2D[3][0]);
	ReturnMat.Arr2D[0][1] = (Arr2D[0][0] * _MAT.Arr2D[0][1]) + (Arr2D[0][1] * _MAT.Arr2D[1][1]) + (Arr2D[0][2] * _MAT.Arr2D[2][1]) + (Arr2D[0][3] * _MAT.Arr2D[3][1]);
	ReturnMat.Arr2D[0][2] = (Arr2D[0][0] * _MAT.Arr2D[0][2]) + (Arr2D[0][1] * _MAT.Arr2D[1][2]) + (Arr2D[0][2] * _MAT.Arr2D[2][2]) + (Arr2D[0][3] * _MAT.Arr2D[3][2]);
	ReturnMat.Arr2D[0][3] = (Arr2D[0][0] * _MAT.Arr2D[0][3]) + (Arr2D[0][1] * _MAT.Arr2D[1][3]) + (Arr2D[0][2] * _MAT.Arr2D[2][3]) + (Arr2D[0][3] * _MAT.Arr2D[3][3]);

	ReturnMat.Arr2D[1][0] = (Arr2D[1][0] * _MAT.Arr2D[0][0]) + (Arr2D[1][1] * _MAT.Arr2D[1][0]) + (Arr2D[1][2] * _MAT.Arr2D[2][0]) + (Arr2D[1][3] * _MAT.Arr2D[3][0]);
	ReturnMat.Arr2D[1][1] = (Arr2D[1][0] * _MAT.Arr2D[0][1]) + (Arr2D[1][1] * _MAT.Arr2D[1][1]) + (Arr2D[1][2] * _MAT.Arr2D[2][1]) + (Arr2D[1][3] * _MAT.Arr2D[3][1]);
	ReturnMat.Arr2D[1][2] = (Arr2D[1][0] * _MAT.Arr2D[0][2]) + (Arr2D[1][1] * _MAT.Arr2D[1][2]) + (Arr2D[1][2] * _MAT.Arr2D[2][2]) + (Arr2D[1][3] * _MAT.Arr2D[3][2]);
	ReturnMat.Arr2D[1][3] = (Arr2D[1][0] * _MAT.Arr2D[0][3]) + (Arr2D[1][1] * _MAT.Arr2D[1][3]) + (Arr2D[1][2] * _MAT.Arr2D[2][3]) + (Arr2D[1][3] * _MAT.Arr2D[3][3]);

	ReturnMat.Arr2D[2][0] = (Arr2D[2][0] * _MAT.Arr2D[0][0]) + (Arr2D[2][1] * _MAT.Arr2D[1][0]) + (Arr2D[2][2] * _MAT.Arr2D[2][0]) + (Arr2D[2][3] * _MAT.Arr2D[3][0]);
	ReturnMat.Arr2D[2][1] = (Arr2D[2][0] * _MAT.Arr2D[0][1]) + (Arr2D[2][1] * _MAT.Arr2D[1][1]) + (Arr2D[2][2] * _MAT.Arr2D[2][1]) + (Arr2D[2][3] * _MAT.Arr2D[3][1]);
	ReturnMat.Arr2D[2][2] = (Arr2D[2][0] * _MAT.Arr2D[0][2]) + (Arr2D[2][1] * _MAT.Arr2D[1][2]) + (Arr2D[2][2] * _MAT.Arr2D[2][2]) + (Arr2D[2][3] * _MAT.Arr2D[3][2]);
	ReturnMat.Arr2D[2][3] = (Arr2D[2][0] * _MAT.Arr2D[0][3]) + (Arr2D[2][1] * _MAT.Arr2D[1][3]) + (Arr2D[2][2] * _MAT.Arr2D[2][3]) + (Arr2D[2][3] * _MAT.Arr2D[3][3]);

	ReturnMat.Arr2D[3][0] = (Arr2D[3][0] * _MAT.Arr2D[0][0]) + (Arr2D[3][1] * _MAT.Arr2D[1][0]) + (Arr2D[3][2] * _MAT.Arr2D[2][0]) + (Arr2D[3][3] * _MAT.Arr2D[3][0]);
	ReturnMat.Arr2D[3][1] = (Arr2D[3][0] * _MAT.Arr2D[0][1]) + (Arr2D[3][1] * _MAT.Arr2D[1][1]) + (Arr2D[3][2] * _MAT.Arr2D[2][1]) + (Arr2D[3][3] * _MAT.Arr2D[3][1]);
	ReturnMat.Arr2D[3][2] = (Arr2D[3][0] * _MAT.Arr2D[0][2]) + (Arr2D[3][1] * _MAT.Arr2D[1][2]) + (Arr2D[3][2] * _MAT.Arr2D[2][2]) + (Arr2D[3][3] * _MAT.Arr2D[3][2]);
	ReturnMat.Arr2D[3][3] = (Arr2D[3][0] * _MAT.Arr2D[0][3]) + (Arr2D[3][1] * _MAT.Arr2D[1][3]) + (Arr2D[3][2] * _MAT.Arr2D[2][3]) + (Arr2D[3][3] * _MAT.Arr2D[3][3]);
*/

	return ReturnMat;
}

