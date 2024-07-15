#pragma once
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <assert.h>

#include "GameTime.h"
#include "GameMacro.h"

//DX들어와서 선언하는 라이브러리, 참조하는 헤더
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")


enum class EDIR
{
	DIRR,
	DIRT,
	DIRL,
	DIRB,
	DIRNONE,
	DIRMAX,
};


//라디안,360분법 바꿔주는 상수와 파이가 저장되어 있다.

class GameMath
{
public:
	static const float PI;
	static const float DTOR;
	static const float RTOD;

public:
	GameMath();
	~GameMath();

public:
	static unsigned int FormatByteSize(DXGI_FORMAT _Fmt);
};

class GameRandom
{
private:
	static int m_Data[32];
	static float m_Timer;

private:
	GameRandom()
	{
	}
	~GameRandom()
	{
	}


private:
	static int GetRandom32bit()
	{
		int retValue = 0;
		for (int i = 0; i < 32; ++i)
		{
			m_Data[i] = rand() % 2;
			retValue |= m_Data[i];
			retValue <<= 1;
		}
		return retValue;
	}
public:
	static void Init()
	{
		srand((unsigned int)time(nullptr));
	}

	static int RandomMinMax(int _Min, int _Max)
	{
		if (_Min == _Max)
		{
			assert(false);
			return _Max;
		}

		if (_Min > _Max)
		{
			assert(false);
			return -1;
		}

		int value = GetRandom32bit();

		if (_Min > value && value < 0)
		{
			value *= -1;
		}

		return value % (_Max + 1 - _Min) + _Min;
	}
};


// 위치 색
class CMatrix;
class CVector
{
public:
	static UINT SX;
	static UINT SY;
	static const CVector ZERO;
	static const CVector ONE;

	static const CVector RED;
	static const CVector GREEN;
	static const CVector BLUE;
	static const CVector WHITE;

	static const CVector LEFT;
	static const CVector RIGHT;
	static const CVector UP;
	static const CVector DOWN;
	static const CVector FORWARD;
	static const CVector BACK;

	// SVEC4는 위치 표현용으로 또는 색 표현용으로 쓰일 수 있다.(union 선언 이유)
	// unNamed 방식의 선언은 그냥 멤버변수인 것처럼 접근이 가능하다.
	// union 이름.x 이런 형태로 접근을 생략하기 위해서 선언함
	// union은 내부 요소중 가장 큰 요소의 메모리 만큼의 공간을 
	// 다른 요소들이 서로 공유한다.
	// 시작점을 동일하게 만든다고 보면 된다. 배열이나 구조체처럼 순차적으로
	// 멤버가 있으면 각자의 크기만큼 다음 위치로 이동한다.
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float Arr[4];

		DirectX::XMFLOAT3 DXFLOAT3;
		DirectX::XMFLOAT4 DXFLOAT4;
		DirectX::XMVECTOR XMVEC;
	};

public:
	CVector() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {	}
	CVector(float _Value) : x(_Value), y(_Value), z(_Value), w(0.0f) {	}
	CVector(float _x, float _y) : x(_x), y(_y), z(0.0f), w(0.0f) {	}
	CVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(0.0f) {	}
	CVector(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {	}
	CVector(const DirectX::XMVECTOR& _Vec) : XMVEC(_Vec) {	}
	CVector(const CVector& _Vec) : XMVEC(_Vec.XMVEC) {	}


public:
	void EulerToQ()
	{
		XMVEC = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMVEC);
	}


	int ix() {
		return (int)x;
	}

	int iy() {
		return (int)y;
	}

	unsigned int uix() {
		return (unsigned int)x;
	}

	unsigned int uiy() {
		return (unsigned int)y;
	}


public:
	//내적 구하기
	//내적은 교환법칙이 성립한다.
	//내적에 들어가는 두 벡터가 단위벡터이면
	//구한 내적값은 두 벡터사이의 코사인세타값이 된다.
	float Dot3D(const CVector& _Other) const
	{
		float x = 0;
		x = this->CVector::x * _Other.x;
		float y = 0;
		y = this->CVector::y * _Other.y;
		float z = 0;
		z = this->CVector::z * _Other.z;
		return x + y + z;
	}

	//외적 구현
	CVector CrossProduct(const CVector& _Other)
	{
		return {
				 y * _Other.z - z * _Other.y,
				 z * _Other.x - x * _Other.z,
				 x * _Other.z - y * _Other.x
		};
	}

	//중심점 구하기(테스트중)
	CVector CentralPos(const CVector& _Other1, const CVector& _Other2)
	{
		CVector NewSVEC4 = { (x + _Other1.x + _Other2.x) / 3,(y + _Other1.y + _Other2.y) / 3,(z + _Other1.z + _Other2.z) / 3 };
		return NewSVEC4;
	}

	float VectorAngleDeg(const CVector& _Other)
	{
		return VectorAngleRad(_Other) * GameMath::RTOD;
	}

	float VectorAngleRad(const CVector& _Other)
	{
		CVector Left = *this;
		CVector Right = _Other;
		//단위벡터화
		Left.Normal3D();
		Right.Normal3D();

		float CosSeta = Left.Dot3D(Right);
		return acosf(CosSeta);
	}

	// 반지름이 1인 원을 기준으로 360도 위치를 받아 x,y를 설정해줌
	static CVector VectorDegToDir2D(float _Degree)
	{
		CVector Pos;
		Pos.DegToDir2D(_Degree);
		//값 복사, 복사생성자, 생성자 호출
		Pos.w = 1.0f;
		return Pos;
	}

	unsigned int ColorToUint()
	{
		UINT color;
		unsigned char * p = (unsigned char*)&color;
		p[0] = (unsigned char)(r * 255);
		p[1] = (unsigned char)(g * 255);
		p[2] = (unsigned char)(b * 255);
		p[3] = (unsigned char)(a * 255);

		return color;
	}

	//각도를 입력받아서 단위 벡터를 자신의 멤버 x,y로 세팅함
	void DegToDir2D(float _Degree)
	{
		RadToDir2D(_Degree * GameMath::DTOR);
	}

	// 3D는 어떻게 할까?
	// 성분표시법으로 벡터의 x,y를 나타낼때
	// 각 x,y좌표는 빗변의 길이가 1인 원을 기준으로 할때 
	// 다음과 같은 비율로 가지게 된다.
	// 6.28을 넘어가면 다시 0부터 자동으로 돌아감
	void RadToDir2D(float _Rad)
	{
		x = cosf(_Rad);
		y = sinf(_Rad);
		return;
	}

	//X,Y,Z축 회전

	void DegToRot2DX(float _Rad)
	{
		RadToRot2DX(_Rad * GameMath::DTOR);
	}

	// 3D는 어떻게 할까?
	void RadToRot2DX(float _Rad)
	{
		// DirectX::xmvector

		// 구면선형보간
		float fz = z * cosf(_Rad) - y * sinf(_Rad);
		float fy = z * sinf(_Rad) + y * cosf(_Rad);
		z = fz;
		y = fy;
		return;
	}

	void DegToRot2DY(float _Rad)
	{
		RadToRot2DY(_Rad * GameMath::DTOR);
	}

	// 3D는 어떻게 할까?
	void RadToRot2DY(float _Rad)
	{
		// DirectX::xmvector

		// 구면선형보간
		float fx = x * cosf(_Rad) - z * sinf(_Rad);
		float fz = x * sinf(_Rad) + z * cosf(_Rad);
		x = fx;
		z = fz;
		return;
	}

	void DegToRot2DZ(float _Rad)
	{
		RadToRot2DZ(_Rad * GameMath::DTOR);
	}

	// 3D는 어떻게 할까?
	void RadToRot2DZ(float _Rad)
	{
		// DirectX::xmvector

		// 구면선형보간
		float fx = x * cosf(_Rad) - y * sinf(_Rad);
		float fy = x * sinf(_Rad) + y * cosf(_Rad);
		x = fx;
		y = fy;
		return;
	}




	// 벡터 크기의 절반값 리턴해주는 함수
	CVector HV() const
	{
		// this의 형태는???
		// HVEC4* const
		// 그런데 함수에 const가 붙으면
		// const HVEC4* const Pos;
		CVector Return = *this;
		Return *= 0.5f;

		return Return;
	}

	//절대값 리턴
	CVector ABS() const
	{
		CVector returnValue = *this;

		returnValue = DirectX::XMVectorAbs(returnValue);

		return returnValue;
	}


	// 정규화가 중요한데.
	// 1을 기준으로 한 비율
	float Len2D()
	{
		float Len = (x * x) + (y * y);
		return sqrtf(Len);
	}

	float Len3D()
	{
		CVector Return = DirectX::XMVector3Length(XMVEC);
		return Return.XMVEC.m128_f32[0];
		//// dirxc
		//float Len = (x * x) + (y * y) + (z * z);
		//return sqrtf(Len);
	}

	//w를 훼손하지 않고 옮기기 위한 값복사
	void Copy2D(const CVector& _Pos)
	{
		x = _Pos.x;
		y = _Pos.y;
	}

	void Copy3D(const CVector& _Pos)
	{
		x = _Pos.x;
		y = _Pos.y;
		z = _Pos.z;
	}


	//단위벡터 3D
	void Normal3D()
	{
		XMVEC = DirectX::XMVector3Normalize(XMVEC);
		//float Len = Len3D();

		//if (0.0f == Len)
		//	// 제로디비전을 미리 막는다.
		//{
		//	ASSERTMSG;
		//	return;
		//}

		//x /= Len;
		//y /= Len;
		//z /= Len;

		//return;
	}

	CVector Normal3DReturn()
	{
		return DirectX::XMVector3Normalize(XMVEC);
	}

	//벡터의 회전, 2D용으로 행렬을 내적값을 그대로 표현
	//공식은 cos, -sin
	//      sin,  cos 곱해주기
	void Rotate2DbyDegree(float _Degree)
	{
		Rotate2DbyRad(GameMath::DTOR *_Degree);
	}

	void Rotate2DbyRad(float _Rad)
	{
		float tempX = 0;
		float tempY = 0;

		tempX = x * cosf(_Rad) + y * sinf(_Rad);
		tempY = -x * sinf(_Rad) + y * cosf(_Rad);

		x = tempX;
		y = tempY;
	}

public:
	//형변환 연산자
	operator POINT()
	{
		POINT P;
		P.x = (int)x;
		P.y = (int)y;
		return P;
	}

	operator const DirectX::XMVECTOR&() const
	{
		return XMVEC;
	}

	operator const DirectX::XMFLOAT3&() const
	{
		return DXFLOAT3;
	}

	operator const DirectX::XMFLOAT4&() const
	{
		return DXFLOAT4;
	}


	//벡터와 행렬의 곱 오퍼레이터
	CVector operator*(const CMatrix& _MAT);
	CVector operator*=(const CMatrix& _MAT);
	//w를 1로 만드는 연산
	CVector TransCoord(const CMatrix& _MAT);
	//w를 0으로 만드는 연산
	CVector TransNormal(const CMatrix& _MAT);

public:
	CVector operator-()
	{
		CVector Return = { 0 };

		Return.x = x * -1;
		Return.y = y * -1;
		Return.z = z * -1;
		Return.w = w * -1;

		return Return;
	}

	CVector operator-(const CVector _Value)
	{
		CVector Return = XMVEC;
		Return.x = x - _Value.x;
		Return.y = y - _Value.y;
		Return.z = z - _Value.z;
		Return.w = w - _Value.w;

		return Return;
	}


	CVector operator+(const CVector _Value)
	{
		CVector Return = XMVEC;
		Return.x = x + _Value.x;
		Return.y = y + _Value.y;
		Return.z = z + _Value.z;
		Return.w = w + _Value.w;

		return Return;
	}


	CVector operator*(const float _Value)
	{
		CVector Return = { 0 };
		Return.x = x * _Value;
		Return.y = y * _Value;
		Return.z = z * _Value;
		// w += _Value.w;나중에 설명

		return Return;
	}

	CVector& operator*=(const float _Value)
	{
		x *= _Value;
		y *= _Value;
		z *= _Value;
		// w += _Value.w;나주에 설명

		return *this;
	}

	CVector operator*(const CVector& _Value)
	{
		CVector Return = { 0 };
		Return.x = x * _Value.x;
		Return.y = y * _Value.y;
		Return.z = z * _Value.z;
		// w += _Value.w;나중에 설명

		return Return;
	}

	CVector& operator*=(const CVector& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;
		z *= _Value.z;
		// w += _Value.w;나주에 설명

		return *this;
	}


	CVector& operator+=(const CVector& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		z += _Value.z;
		w += _Value.w;

		return *this;
	}

	CVector& operator/=(const int& _Value)
	{
		x /= _Value;
		y /= _Value;
		z /= _Value;
		w /= _Value;

		return *this;
	}


	bool operator!=(const CVector& _Value) const
	{
		// w += _Value.w;나주에 설명

		return 	x != _Value.x ||
			y != _Value.y ||
			z != _Value.z;
	}
};


//매트릭스를 정의함
class CMatrix
{
public:
	static UINT SX;
	static UINT SY;

	union
	{
		float Arr2D[4][4];
		CVector ArrVec[4];
		float Arr1D[16];
		struct
		{
			float _00; float _01; float _02; float _03;
			float _10; float _11; float _12; float _13;
			float _20; float _21; float _22; float _23;
			float _30; float _31; float _32; float _33;
		};

		DirectX::XMMATRIX XMMAT;
	};

public:
	static const CMatrix LaplaceMat3D;

public:
	CMatrix() {}
	CMatrix(CVector _One, CVector _Two, CVector _Three, CVector _Four);
	CMatrix(const CMatrix& _Other) : XMMAT(_Other.XMMAT) {}
	~CMatrix() {}

public:
	CMatrix operator*(const CMatrix& _MAT);

	void IDEN()
	{
		XMMAT = DirectX::XMMatrixIdentity();

		//// 단위행렬
		//memset(Arr1D, 0, sizeof(CMatrix));

		//for (size_t i = 0; i < SX; i++)
		//{
		//	Arr2D[i][i] = 1.0f;
		//}
	}

	void Pos(const CVector& _Pos)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
		//ArrVec[3].Copy3D(_Pos);
	}

	void Scale(const CVector& _Scale)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
		/*for (size_t i = 0; i < SX - 1; i++)
		{
			Arr2D[i][i] = _Scale.Arr[i];
		}*/
	}

	//360분법이 사람에게 익숙하니까 입력받지만 실제론 라디안이 근원쪽이다
	void RotXDeg(const float _Value)
	{
		RotXRad(_Value * GameMath::DTOR);
	}

	void RotYDeg(const float _Value)
	{
		RotYRad(_Value * GameMath::DTOR);
	}

	void RotZDeg(const float _Value)
	{
		RotZRad(_Value * GameMath::DTOR);
	}

	void RotXRad(const float _Value)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixRotationX(_Value);
		/*
				Arr2D[1][1] = cosf(_Value);
				Arr2D[1][2] = -sinf(_Value);
				Arr2D[2][1] = sinf(_Value);
				Arr2D[2][2] = cosf(_Value);*/
	}

	void RotYRad(const float _Value)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixRotationY(_Value);

		/*Arr2D[0][0] = cosf(_Value);
		Arr2D[0][2] = sinf(_Value);
		Arr2D[2][0] = -sinf(_Value);
		Arr2D[2][2] = cosf(_Value);*/
	}

	void RotZRad(const float _Value)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixRotationZ(_Value);

		/*Arr2D[0][0] = cosf(_Value);
		Arr2D[0][1] = -sinf(_Value);
		Arr2D[1][0] = sinf(_Value);
		Arr2D[1][1] = cosf(_Value);*/
	}

	// LH, RH 
	// _EyePos 내가 보고 있는 위치
	void VIEWTOLH(const CVector& _EyePos, const CVector& _EyeDir, const CVector& _EyeUp)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp);
		//CVector Dir = _EyeDir;
		//CVector Up = _EyeUp;

		////z축
		//ArrVec[2] = Dir;
		////y축
		//ArrVec[1] = Up;
		//// 회전행렬이 반전된 회전행렬 x 를 40 회전시키면
		//// 반전시키면 x를 -402
		//ArrVec[0] = ArrVec[1].CrossProduct(ArrVec[2]);
		//TransPos();
		//

		//ArrVec[3] =
		//	CVector(
		//	      ArrVec[0].Arr[0] * _EyePos.Arr[0] 
		//		+ ArrVec[1].Arr[0] * _EyePos.Arr[1] 
		//		+ ArrVec[2].Arr[0] * _EyePos.Arr[2],
		//	   ArrVec[1].Dot3D(_EyePos),
		//	   ArrVec[2].Dot3D(_EyePos),
		//	   1.0F
		//	);

		//ArrVec[2] = Dir;
		//ArrVec[1] = Up;
		//ArrVec[0] = ArrVec[1].CrossProduct(ArrVec[2]);
		//// 회전행렬이 반전된 회전행렬 x 를 40 회전시키면
		//// 반전시키면 x를 -40
		//TransPos();



		//ArrVec[3].Arr[0] = ArrVec[3].Arr[0] * -1;
		//ArrVec[3].Arr[1] = ArrVec[3].Arr[1] * -1;
		//ArrVec[3].Arr[2] = ArrVec[3].Arr[2] * -1;



		//ArrVec[3] =
		//	CVector(
		//		-ArrVec[0].Dot3D(_EyePos),
		//		-ArrVec[1].Dot3D(_EyePos),
		//		-ArrVec[2].Dot3D(_EyePos),
		//		1.0F
		//	);


		//ArrVec[2] = Dir;
		//ArrVec[1] = Up;
		//// 회전행렬이 반전된 회전행렬 x 를 40 회전시키면
		//// 반전시키면 x를 -402
		//TransPos();


		//ArrVec[3] =
		//	CVector(
		//	   ArrVec[0].Arr[0] * _EyePos.Arr[0] + ArrVec[1].Arr[0] * _EyePos.Arr[1] + ArrVec[2].Arr[0] * _EyePos.Arr[2],
		//	   ArrVec[1].Dot3D(_EyePos),
		//	   ArrVec[2].Dot3D(_EyePos),
		//	   1.0F
		//	);

		//ArrVec[3].Arr[0] = ArrVec[3].Arr[0] * -1;
		//ArrVec[3].Arr[1] = ArrVec[3].Arr[1] * -1;
		//ArrVec[3].Arr[2] = ArrVec[3].Arr[2] * -1;

		//XMMAT = DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp);

	}

	void ORTHLH(CVector _Size, float _Near, float _Far)
	{
		ORTHLH(_Size.x, _Size.y, _Near, _Far);
	}

	//직교 투영 함수
	//높이와 너비를 내 맘대로 조절할 수 있다.
	//하지만 이 부분이 어떻게 개별 좌표계가 되는지는 모르겠는데?
	void ORTHLH(float _Width, float _Height, float _Near, float _Far)
	{
		IDEN();

		//float w = 2.0f / _Width;
		//float h = 2.0f / _Height;
		//float a = 1.0f;
		//float b = 0.0f;
		//float fMinusN = _Far - _Near;

		//Arr2D[0][0] = w;
		//Arr2D[1][1] = h;
		//Arr2D[2][2] = 1/ fMinusN;
		//Arr2D[3][2] = _Near / fMinusN;
		//Arr2D[3][3] = 1.0f;

		XMMAT = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
	}

	//원근 투영 함수
	void PERSLHDEG(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		PERSLHRAD(_Fov * GameMath::DTOR, _Width, _Height, _Near, _Far);
	}

	void PERSLHRAD(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		IDEN();

		XMMAT = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far);

		// a
		//float Ratio = _Width / _Height;
		//float HR = _Fov;
		//float XR = _Fov * Ratio;

		//// [1 / Ratio * (tanf(A / 2))] [   ] [   ] [    ] 
		//// [						 ] [   ] [   ] [    ] 
		//// [						 ] [   ] [   ] [1.0f] 
		//// [						 ] [   ] [   ] [    ] 


		//// 100 1100

		//// 이건다 비율값인거 아시죠?
		//Arr2D[0][0] = 1 / (Ratio * (tanf(HR / 2)));
		//Arr2D[1][1] = 1 / (tanf(HR / 2));
		////            1100 / 1000
		//Arr2D[2][2] = _Far / (_Far - _Near);
		//// -110 / 1000
		//Arr2D[3][2] = (-_Near * _Far) / (_Far - _Near);

		//// 이건 점이랑 곱해졌을때 W위치에 Z값이 들어가게 만들려고 하는것.
		//Arr2D[2][3] = 1.0f; // 점이랑 곱해졌을대 
		//// 점의 W에 Z값이 들어가있게 만들려고 한것이다.

		//// w값에 1이 더해진다.
		//// 아래 이값에 넣어주지 않으면
		//// 91
		//Arr2D[3][3] = 0.0f; // 점이랑 곱해졌을대 
		// 값을 남겨놨다가
		// z를 w로 나누면서 
		// 0~1사이의 공간으로 보내버린다.
	}

	// BitBit
	// 화면 중앙을 0, 0
	void VIEWPORT(float _StartX, float _StartY, float _Width, float _Height, float _MinZ, float _MaxZ)
	{
		//어차피 뷰포트값은 외부에서 넣어주므로 이 함수를 호출할 일은 거의 없다.
		IDEN();

		Arr2D[0][0] = _Width / 2;
		Arr2D[1][1] = -(_Height / 2); // y축 반전
		Arr2D[2][2] = _MaxZ - _MinZ; // 큰 의미는 없다.

		Arr2D[3][0] = _StartX + Arr2D[0][0];
		Arr2D[3][1] = _StartX + (_Height / 2);
		Arr2D[3][2] = _MinZ;
	}


	void TransPos()
	{
		XMMAT = DirectX::XMMatrixTranspose(XMMAT);
		/*
				float Temp = 0.0f;

				Temp = Arr2D[0][1];
				Arr2D[0][1] = Arr2D[1][0];
				Arr2D[1][0] = Temp;
				Temp = Arr2D[0][2];		Arr2D[0][2] = Arr2D[2][0];		Arr2D[2][0] = Temp;
				Temp = Arr2D[0][3];		Arr2D[0][3] = Arr2D[3][0];		Arr2D[3][0] = Temp;
				Temp = Arr2D[1][2];		Arr2D[1][2] = Arr2D[2][1];		Arr2D[2][1] = Temp;
				Temp = Arr2D[1][3];		Arr2D[1][3] = Arr2D[3][1];		Arr2D[3][1] = Temp;
				Temp = Arr2D[2][3];		Arr2D[2][3] = Arr2D[3][2];		Arr2D[3][2] = Temp;*/
	}
};

struct GameRect
{
	union
	{
		struct
		{
			CVector LeftTop;
			CVector RightBottom;
		};
	};

	GameRect() {}
	GameRect(const GameRect& _Other) 
	{
		LeftTop = _Other.LeftTop;
		RightBottom = _Other.RightBottom;
	}
	GameRect(const CVector& _Left, const CVector& _Right)
	{
		LeftTop = _Left;
		RightBottom = _Right;
	}
	~GameRect() {}
};

struct CMatrixData
{
	union
	{
		struct
		{
			CVector One;
			CVector Two;
			CVector Three;
			CVector Four;
		};
		CMatrix Mat;
	};

	void AllTP()
	{
		Mat.TransPos();
	}

	CMatrixData() {	}
	CMatrixData(const CMatrixData& _Other) : Mat(_Other.Mat) {	}
	CMatrixData(const CMatrix& _Other) : Mat(_Other) {	}
	~CMatrixData() {	}
};

struct CTransData
{
	union
	{
		struct
		{
			CMatrix W;
			CMatrix V;
			CMatrix P;
			CMatrix WV;
			CMatrix VP;
			CMatrix WVP;
		};
		CMatrix Arr[6];
	};

public:
	void AllTP()
	{
		for (size_t i = 0; i < 6; i++)
		{
			Arr[i].TransPos();
		}
	}

	CTransData() {	}
	CTransData(const CTransData& _Other)
		:
		W(_Other.W),
		V(_Other.V),
		P(_Other.P),
		WV(_Other.WV),
		VP(_Other.VP),
		WVP(_Other.WVP)
	{
	}
	~CTransData() {	}
};