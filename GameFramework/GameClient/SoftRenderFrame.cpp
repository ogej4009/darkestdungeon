#include "SoftRenderFrame.h"
#include <Windows.h>
#include <GameWin.h>
#include <GameTime.h>
#include <GameInput.h>

SoftRenderFrame::SoftRenderFrame()
{
}


SoftRenderFrame::~SoftRenderFrame()
{
}

#define RCOUNT 4
#define RCCOUNT 6

CVector CenterPos = { 0,0 };
CVector LocalPos[RCCOUNT][RCOUNT];
CVector WorldPos[RCCOUNT][RCOUNT];
POINT ConvertPos[RCCOUNT][RCOUNT];

//사각형 초기 세팅
CVector Scale = { 100.0f, 100.0f, 100.0f };
CVector Rot = { 0, 0, 0 };
CVector Pos = { 0.0f, 0.0F, 0.0F };

//카메라 초기 세팅
CVector ViewPos = { 0.0f , 0.0f, -300.0f };
CVector ViewRot = { 0.0f , 0.0f, 0.0f };


float Speed = 100.0f;
float Near = 0.1f; // 근평면 거리
float Far = 10000.0f; // 원평면 거리
float Width = 1024; // 너비
float Height = 768; // 높이
float _Fov = 60.0f; // 시야각

//카메라 정보
CMatrix MScale;
CMatrix MRotX;
CMatrix MRotY;
CMatrix MRotZ;
CMatrix MRot;
CMatrix MPos;
CMatrix MWorld;
CMatrix MMVP;
CMatrix MVIEWPORT;

//뷰행렬 정보
CMatrix MViewRotX;
CMatrix MViewRotY;
CMatrix MViewRotZ;
CMatrix MViewRot;
CMatrix MView;


//투영행렬
CMatrix MProj;


void SoftRenderFrame::Init()
{
	/*GameInput::CreateKey(L"LMOVE", 'A');
	GameInput::CreateKey(L"RMOVE", 'D');
	GameInput::CreateKey(L"UMOVE", 'W');
	GameInput::CreateKey(L"DMOVE", 'S');
	GameInput::CreateKey(L"FMOVE", 'Q');
	GameInput::CreateKey(L"BMOVE", 'E');*/

	GameInput::CreateKey(L"ROTX", 'T');
	GameInput::CreateKey(L"ROTY", 'G');
	GameInput::CreateKey(L"ROTZ", 'B');
	GameInput::CreateKey(L"ROTMX", 'Y');
	GameInput::CreateKey(L"ROTMY", 'H');
	GameInput::CreateKey(L"ROTMZ", 'N');

	GameInput::CreateKey(L"SCALX", 'U');
	GameInput::CreateKey(L"SCALY", 'J');
	GameInput::CreateKey(L"SCALZ", 'M');
	GameInput::CreateKey(L"SCALMX", 'I');
	GameInput::CreateKey(L"SCALMY", 'K');
	GameInput::CreateKey(L"SCALMZ", VK_OEM_COMMA);

	GameInput::CreateKey(L"LCAMMOVE", VK_NUMPAD4);
	GameInput::CreateKey(L"RCAMMOVE", VK_NUMPAD6);
	GameInput::CreateKey(L"UCAMMOVE", VK_NUMPAD8);
	GameInput::CreateKey(L"DCAMMOVE", VK_NUMPAD5);
	GameInput::CreateKey(L"FCAMMOVE", VK_NUMPAD7);
	GameInput::CreateKey(L"BCAMMOVE", VK_NUMPAD9);

	GameInput::CreateKey(L"CAMROTX", '1');
	GameInput::CreateKey(L"CAMROTY", '2');
	GameInput::CreateKey(L"CAMROTZ", '3');
	GameInput::CreateKey(L"CAMROTMX", '4');
	GameInput::CreateKey(L"CAMROTMY", '5');
	GameInput::CreateKey(L"CAMROTMZ", '6');


	CenterPos = GameWin::MainWindow()->Size().HV();

	//로컬스페이스
	//w값에 항상 주의해라. 빼먹으면 안움직인다.
	//로컬 세팅하는 부분을 렌더링 파이프라인에서 인풋 어셈블러 과정이라고 볼 수 있다.

	//앞면 시계방향
	LocalPos[0][0] = { 0.5f, 0.5f, 0.5f ,1 };
	LocalPos[0][1] = { 0.5f, -0.5f, 0.5f ,1 };
	LocalPos[0][2] = { -0.5f, -0.5f, 0.5f,1 };
	LocalPos[0][3] = { -0.5f, 0.5f, 0.5f,1 };

	//뒷면
	LocalPos[1][0] = { 0.5f, 0.5f, -0.5f,1 };
	LocalPos[1][1] = { 0.5f, -0.5f, -0.5f ,1 };
	LocalPos[1][2] = { -0.5f, -0.5f, -0.5f ,1 };
	LocalPos[1][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//왼쪽면
	LocalPos[2][0] = { -0.5f, 0.5f, 0.5f,1 };
	LocalPos[2][1] = { -0.5f, -0.5f, 0.5f ,1 };
	LocalPos[2][2] = { -0.5f, -0.5f, -0.5f ,1 };
	LocalPos[2][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//오른쪽면
	LocalPos[3][0] = { 0.5f, 0.5f, 0.5f,1 };
	LocalPos[3][1] = { 0.5f, -0.5f, 0.5f ,1 };
	LocalPos[3][2] = { 0.5f, -0.5f, -0.5f ,1 };
	LocalPos[3][3] = { 0.5f, 0.5f, -0.5f,1 };

	//윗면
	LocalPos[4][0] = { 0.5f, 0.5f, -0.5f,1 };
	LocalPos[4][1] = { 0.5f, 0.5f, 0.5f,1 };
	LocalPos[4][2] = { -0.5f, 0.5f, 0.5f ,1 };
	LocalPos[4][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//아랫면
	LocalPos[5][0] = { 0.5f, -0.5f, -0.5f,1 };
	LocalPos[5][1] = { 0.5f, -0.5f, 0.5f,1 };
	LocalPos[5][2] = { -0.5f, -0.5f, 0.5f ,1 };
	LocalPos[5][3] = { -0.5f, -0.5f, -0.5f ,1 };


}


void WINAPI SoftRenderFrame::LogicUpdate()
{
	GameTime::Update();
	//GameInput::Update() 매 프레임마다 눌렸는지 여부를 체크하는 함수
	GameInput::Update();

	if (true == GameInput::Press(L"RMOVE"))
	{
		Pos.x += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"LMOVE"))
	{
		Pos.x -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"UMOVE"))
	{
		Pos.y += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"DMOVE"))
	{
		Pos.y -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"FMOVE"))
	{
		Pos.z += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"BMOVE"))
	{
		Pos.z -= Speed * GameTime::DeltaTime();
	}

	if (true == GameInput::Press(L"ROTX"))
	{
		Rot.x += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"ROTY"))
	{
		Rot.y += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"ROTZ"))
	{
		Rot.z += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"ROTMX"))
	{
		Rot.x -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"ROTMY"))
	{
		Rot.y -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"ROTMZ"))
	{
		Rot.z -= Speed * GameTime::DeltaTime();
	}

	if (true == GameInput::Press(L"SCALX"))
	{
		Scale.x += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"SCALY"))
	{
		Scale.y += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"SCALZ"))
	{
		Scale.z += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"SCALMX"))
	{
		Scale.x -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"SCALMY"))
	{
		Scale.y -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"SCALMZ"))
	{
		Scale.z -= Speed * GameTime::DeltaTime();
	}

	// CAM
	if (true == GameInput::Press(L"RCAMMOVE"))
	{
		ViewPos.x += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"LCAMMOVE"))
	{
		ViewPos.x -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"UCAMMOVE"))
	{
		ViewPos.y += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"DCAMMOVE"))
	{
		ViewPos.y -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"FCAMMOVE"))
	{
		ViewPos.z += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"BCAMMOVE"))
	{
		ViewPos.z -= Speed * GameTime::DeltaTime();
	}

	if (true == GameInput::Press(L"CAMROTX"))
	{
		ViewRot.x += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"CAMROTY"))
	{
		ViewRot.y += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"CAMROTZ"))
	{
		ViewRot.z += Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"CAMROTMX"))
	{
		ViewRot.x -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"CAMROTMY"))
	{
		ViewRot.y -= Speed * GameTime::DeltaTime();
	}
	if (true == GameInput::Press(L"CAMROTMZ"))
	{
		ViewRot.z -= Speed * GameTime::DeltaTime();
	}
}


void WINAPI SoftRenderFrame::Progress()
{
	//인풋어셈블리 과정
	LogicUpdate();

	//X,Y,Z가 다 각각 Scale 벡터값을 가지게 된다.
	MScale.Scale(Scale);

	MRotX.RotXDeg(Rot.x);
	MRotY.RotYDeg(Rot.y);
	MRotZ.RotZDeg(Rot.z);
	//회전 행렬 만드는 방식 
	//x 회전행렬 * y 회전행렬 * z 회전행렬
	MRot = MRotX * MRotY * MRotZ;

	//회전행렬을 단위화 시킴
	MRot.ArrVec[0].Normal3D();
	MRot.ArrVec[1].Normal3D();
	MRot.ArrVec[2].Normal3D();

	//데이터 테스트용 코드로 보임
	float LenX = MRot.ArrVec[0].Len3D();
	float LenY = MRot.ArrVec[1].Len3D();
	float LenZ = MRot.ArrVec[2].Len3D();

	//위치 세팅, x,y,z 대입
	MPos.Pos(Pos);

	// 월드에서의 최종 행렬은
	// 크자이, 크기 자전(회전), 이동
	// 버텍스 과정중 월드 과정
	MWorld = MScale * MRot * MPos;


	//뷰행렬 세팅
	//뷰 시점의 회전행렬 만들어 주기
	MViewRotX.RotXDeg(ViewRot.x);
	MViewRotY.RotYDeg(ViewRot.y);
	MViewRotZ.RotZDeg(ViewRot.z);
	MViewRot = MViewRotX * MViewRotY * MViewRotZ;

	MView.VIEWTOLH(ViewPos, MViewRot.ArrVec[2], MViewRot.ArrVec[1]);
	//원근 투영행렬 적용
	MProj.PERSLHDEG(_Fov, Width, Height, Near, Far);
	//직교 투영행렬 만들어보기
	//MProj.OrthProj(Width, Height);

	MVIEWPORT.VIEWPORT(0, 0, 1024.0f, 768.0f, 0.0f, 1.0f);

	MWorld = MWorld * MView * MProj;

	//월드포스로 시작 값을 받아옴
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			WorldPos[i][j] = LocalPos[i][j];
		}
	}


	//시작 벡터와 행렬간의 연산으로 크기,회전,이동을 다 연산함
	// 원점을 기준으로 있을때 돌리는 것이 회전.
	for (size_t i = 0; i < RCCOUNT; i++)
	{
		for (size_t j = 0; j < RCOUNT; j++)
		{
			// 이건 어쩔수 없이 한것.
			WorldPos[i][j] *= MWorld;
			//
		}
	}


	//투영
	//투영행렬에서 남겨놓은 z값으로 나눠줌
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			float ViewZ = WorldPos[i][j].w;
			WorldPos[i][j].x /= ViewZ;
			WorldPos[i][j].y /= ViewZ;
			WorldPos[i][j].z /= ViewZ;
			WorldPos[i][j].w = 1.0f;
		}
	}

	//뷰포트
	//그냥 곱해주면 됨
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			// 이건 어쩔수 없이 한것.
			WorldPos[i][j] = WorldPos[i][j] * MVIEWPORT;
		}
	}


	//Polygon이 받는 매개변수로 옮기기
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			//형변환 연산자 오퍼레이팅 되어있음
			ConvertPos[i][j] = WorldPos[i][j];
		}
	}


	for (int i = 0; i < RCCOUNT; i++)
	{
		Polygon(GameWin::MainHDC(), ConvertPos[i], RCOUNT);
	}


	//// 초기위치 잡는것.
	//// 로컬
	////DegToDir 함수에서 라디안값을 받아서 
	////단위벡터로 만들어진 x,y값을 리턴해줌
	////Deg가 float 최대값을 넘어가는 경우 지수가 255가 되서 무한이나 Nan을 표현할 수 있다.
	//RPos.DegToDir(360.0f * Deg);


	//// 월드
	//// 크기
	//// 단위벡터에 곱하기 100을 해서 크기를 키워줌
	//RPos *= 100.0f;
	//// 회전

	//// 이동
	//// Pos는 중심위치를 의미하며 더했으므로 중심위치만큼 더해서 이동함
	//RPos += Pos;

	//Rectangle(GameWin::MainHDC(), RPos.ix() - 5, RPos.iy() - 5,
	//	RPos.ix() + 5, RPos.iy() + 5);

}