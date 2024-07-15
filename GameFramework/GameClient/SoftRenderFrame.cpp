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

//�簢�� �ʱ� ����
CVector Scale = { 100.0f, 100.0f, 100.0f };
CVector Rot = { 0, 0, 0 };
CVector Pos = { 0.0f, 0.0F, 0.0F };

//ī�޶� �ʱ� ����
CVector ViewPos = { 0.0f , 0.0f, -300.0f };
CVector ViewRot = { 0.0f , 0.0f, 0.0f };


float Speed = 100.0f;
float Near = 0.1f; // ����� �Ÿ�
float Far = 10000.0f; // ����� �Ÿ�
float Width = 1024; // �ʺ�
float Height = 768; // ����
float _Fov = 60.0f; // �þ߰�

//ī�޶� ����
CMatrix MScale;
CMatrix MRotX;
CMatrix MRotY;
CMatrix MRotZ;
CMatrix MRot;
CMatrix MPos;
CMatrix MWorld;
CMatrix MMVP;
CMatrix MVIEWPORT;

//����� ����
CMatrix MViewRotX;
CMatrix MViewRotY;
CMatrix MViewRotZ;
CMatrix MViewRot;
CMatrix MView;


//�������
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

	//���ý����̽�
	//w���� �׻� �����ض�. �������� �ȿ����δ�.
	//���� �����ϴ� �κ��� ������ ���������ο��� ��ǲ ����� �����̶�� �� �� �ִ�.

	//�ո� �ð����
	LocalPos[0][0] = { 0.5f, 0.5f, 0.5f ,1 };
	LocalPos[0][1] = { 0.5f, -0.5f, 0.5f ,1 };
	LocalPos[0][2] = { -0.5f, -0.5f, 0.5f,1 };
	LocalPos[0][3] = { -0.5f, 0.5f, 0.5f,1 };

	//�޸�
	LocalPos[1][0] = { 0.5f, 0.5f, -0.5f,1 };
	LocalPos[1][1] = { 0.5f, -0.5f, -0.5f ,1 };
	LocalPos[1][2] = { -0.5f, -0.5f, -0.5f ,1 };
	LocalPos[1][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//���ʸ�
	LocalPos[2][0] = { -0.5f, 0.5f, 0.5f,1 };
	LocalPos[2][1] = { -0.5f, -0.5f, 0.5f ,1 };
	LocalPos[2][2] = { -0.5f, -0.5f, -0.5f ,1 };
	LocalPos[2][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//�����ʸ�
	LocalPos[3][0] = { 0.5f, 0.5f, 0.5f,1 };
	LocalPos[3][1] = { 0.5f, -0.5f, 0.5f ,1 };
	LocalPos[3][2] = { 0.5f, -0.5f, -0.5f ,1 };
	LocalPos[3][3] = { 0.5f, 0.5f, -0.5f,1 };

	//����
	LocalPos[4][0] = { 0.5f, 0.5f, -0.5f,1 };
	LocalPos[4][1] = { 0.5f, 0.5f, 0.5f,1 };
	LocalPos[4][2] = { -0.5f, 0.5f, 0.5f ,1 };
	LocalPos[4][3] = { -0.5f, 0.5f, -0.5f ,1 };

	//�Ʒ���
	LocalPos[5][0] = { 0.5f, -0.5f, -0.5f,1 };
	LocalPos[5][1] = { 0.5f, -0.5f, 0.5f,1 };
	LocalPos[5][2] = { -0.5f, -0.5f, 0.5f ,1 };
	LocalPos[5][3] = { -0.5f, -0.5f, -0.5f ,1 };


}


void WINAPI SoftRenderFrame::LogicUpdate()
{
	GameTime::Update();
	//GameInput::Update() �� �����Ӹ��� ���ȴ��� ���θ� üũ�ϴ� �Լ�
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
	//��ǲ����� ����
	LogicUpdate();

	//X,Y,Z�� �� ���� Scale ���Ͱ��� ������ �ȴ�.
	MScale.Scale(Scale);

	MRotX.RotXDeg(Rot.x);
	MRotY.RotYDeg(Rot.y);
	MRotZ.RotZDeg(Rot.z);
	//ȸ�� ��� ����� ��� 
	//x ȸ����� * y ȸ����� * z ȸ�����
	MRot = MRotX * MRotY * MRotZ;

	//ȸ������� ����ȭ ��Ŵ
	MRot.ArrVec[0].Normal3D();
	MRot.ArrVec[1].Normal3D();
	MRot.ArrVec[2].Normal3D();

	//������ �׽�Ʈ�� �ڵ�� ����
	float LenX = MRot.ArrVec[0].Len3D();
	float LenY = MRot.ArrVec[1].Len3D();
	float LenZ = MRot.ArrVec[2].Len3D();

	//��ġ ����, x,y,z ����
	MPos.Pos(Pos);

	// ���忡���� ���� �����
	// ũ����, ũ�� ����(ȸ��), �̵�
	// ���ؽ� ������ ���� ����
	MWorld = MScale * MRot * MPos;


	//����� ����
	//�� ������ ȸ����� ����� �ֱ�
	MViewRotX.RotXDeg(ViewRot.x);
	MViewRotY.RotYDeg(ViewRot.y);
	MViewRotZ.RotZDeg(ViewRot.z);
	MViewRot = MViewRotX * MViewRotY * MViewRotZ;

	MView.VIEWTOLH(ViewPos, MViewRot.ArrVec[2], MViewRot.ArrVec[1]);
	//���� ������� ����
	MProj.PERSLHDEG(_Fov, Width, Height, Near, Far);
	//���� ������� ������
	//MProj.OrthProj(Width, Height);

	MVIEWPORT.VIEWPORT(0, 0, 1024.0f, 768.0f, 0.0f, 1.0f);

	MWorld = MWorld * MView * MProj;

	//���������� ���� ���� �޾ƿ�
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			WorldPos[i][j] = LocalPos[i][j];
		}
	}


	//���� ���Ϳ� ��İ��� �������� ũ��,ȸ��,�̵��� �� ������
	// ������ �������� ������ ������ ���� ȸ��.
	for (size_t i = 0; i < RCCOUNT; i++)
	{
		for (size_t j = 0; j < RCOUNT; j++)
		{
			// �̰� ��¿�� ���� �Ѱ�.
			WorldPos[i][j] *= MWorld;
			//
		}
	}


	//����
	//������Ŀ��� ���ܳ��� z������ ������
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

	//����Ʈ
	//�׳� �����ָ� ��
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			// �̰� ��¿�� ���� �Ѱ�.
			WorldPos[i][j] = WorldPos[i][j] * MVIEWPORT;
		}
	}


	//Polygon�� �޴� �Ű������� �ű��
	for (int i = 0; i < RCCOUNT; i++)
	{
		for (int j = 0; j < RCOUNT; j++)
		{
			//����ȯ ������ ���۷����� �Ǿ�����
			ConvertPos[i][j] = WorldPos[i][j];
		}
	}


	for (int i = 0; i < RCCOUNT; i++)
	{
		Polygon(GameWin::MainHDC(), ConvertPos[i], RCOUNT);
	}


	//// �ʱ���ġ ��°�.
	//// ����
	////DegToDir �Լ����� ���Ȱ��� �޾Ƽ� 
	////�������ͷ� ������� x,y���� ��������
	////Deg�� float �ִ밪�� �Ѿ�� ��� ������ 255�� �Ǽ� �����̳� Nan�� ǥ���� �� �ִ�.
	//RPos.DegToDir(360.0f * Deg);


	//// ����
	//// ũ��
	//// �������Ϳ� ���ϱ� 100�� �ؼ� ũ�⸦ Ű����
	//RPos *= 100.0f;
	//// ȸ��

	//// �̵�
	//// Pos�� �߽���ġ�� �ǹ��ϸ� �������Ƿ� �߽���ġ��ŭ ���ؼ� �̵���
	//RPos += Pos;

	//Rectangle(GameWin::MainHDC(), RPos.ix() - 5, RPos.iy() - 5,
	//	RPos.ix() + 5, RPos.iy() + 5);

}