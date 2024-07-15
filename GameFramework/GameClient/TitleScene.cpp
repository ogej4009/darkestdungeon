#include "TitleScene.h"
#include <LogicHeader.h>
#include <GameInput.h>
#include <GameActor.h>
#include <GameTransform.h>
#include <GameRenderer.h>
#include <GameCam.h>
#include <GameDic.h>
#include <GameTexture.h>
#include <GameImage.h>
#include <GameAni.h>
#include <GameSprite.h>
#include <GameJsonReader.h>
#include <GameDebugEn.h>
//3D���� ����
#include <GameFreeCam.h>
#include <GameGrid.h>

#include "GameMap.h"
#include "Combat.h"
#include "TextReader.h"
#include "Hero.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Loading()
{

	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"SCJ_DX");
		m_Dir.Move(L"BIN");
		m_Dir.Move(L"TEX");

		Scene()->COLMGR.Link((unsigned int)RENDERGROUP::RG_PLAYER, (unsigned int)RENDERGROUP::RG_MONSTER);

		GameTexture::Load(m_Dir.CreateGameFilePath(L"ShovelKnight.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"ShovelKnight.png"), 5, 3);
	}

	/*{
		HGAMEDIR m_Dir;
		m_Dir.MoveParent(L"HDX");
		m_Dir.Move(L"BIN");
		m_Dir.Move(L"Sound");
	}*/


	{
		CamWin = CreateActor(L"MAINCAM");
		CamWin->Trans()->LPOS({ 0, 0, -5.0f });

		CamWin->AddCom<GameFreeCam>();
		SPtr<GameCam> MyCam = CamWin->AddCom<GameCam>(10, (unsigned int)RENDERGROUP::RG_MAP, (unsigned int)RENDERGROUP::RG_PLAYER, (unsigned int)RENDERGROUP::RG_MONSTER, 10);
		// �۽���Ƽ�� �϶��� ���ٰ��� ǥ���ϱ� ������
		// ������ ���̰� �ȴ�.
		MyCam->ViewType(GameCam::CAMTYPE::PERS);
		MyCam->Size({ 4, 3 });
	}


	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->WPOS({ 0.0F, 0.0F, 0.0F });
		PTR->Trans()->WSCALE({ 100000.0F, 0.0F, 100000.0F });
		GridRender = PTR->AddCom<GameGrid>();
	}

	{
		SPtr<GameActor> PTR = CreateActor(L"�÷��̾�");
		PTR->Trans()->WPOS({ 0, 0, -2.0f });
		PTR->AddCom<Hero>();
	}

	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->WPOS({ -1.5F, 0.0F, 0.0F });
		SPtr<GameImage> RENDER = PTR->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP);
		RENDER->Sprite(L"ShovelKnight.png");
	}
}

void TitleScene::Update()
{

}

void TitleScene::DebugRender()
{
}
//
//#include "STITLECOM.h"
//#include <LOGICHEADER.h>
//#include <SGAMEINPUT.h>
//#include <SGAMEACTOR.h>
//#include <STRANS.h>
//#include <SRENDER.h>
//#include <SCAM.h>
//#include <SGAMEDIR.h>
//#include <STEX.h>
//#include <SIMGRENDER.h>
//#include <S2DANI.h>
//#include <SJSONREADER.h>
//#include <S3DDEBUG.h>
//#include "GameMap.h"
//#include "SGAMECOMBAT.h"
//#include "TextReader.h"
//
//STITLECOM::STITLECOM()
//{
//}
//
//
//STITLECOM::~STITLECOM()
//{
//
//}
//
//void STITLECOM::Loading()
//{
//
//}
//
//void STITLECOM::Update()
//{
//
//
//	if (GameInput::Down(L"Attack"))
//	{
//		//GAMECOMBAT�� �޾Ƴ��� �� ����� ���ִ� ���� �Ŵ���
//		//�ش� ������ FSM�� ó�� ����. �ٵ� FSM�� ��� ó���ؾ� ����?->������ �ѹ� �������.
//		//������ ���͵� FSM�� �������� �κ��� ������ �������� FSMó�� ������Ʈ�� ����� �ʹ�.
//		//UI ��ư �Է� -> FSMó�� ������Ʈ�� �����Ŵ����� ī�޶� ����
//		//FSMó�� ������Ʈ�� ���� ����
//		//�÷��̾� Ŭ������ ���� ������ �ϳ�? ���Ͱ� ������ �� ��� �ִµ�?
//		//m_PlayerPTR->GetCom<SGAMECOMBAT>()->Attack(m_EnemyPTR->GetCom<SGAMECOMBAT>());
//	}
//}
//
//void STITLECOM::DebugRender()
//{
//	GameDebugEn::Draw2DFont(m_FPSstr, 20.0f, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
//
//	//wchar_t buff[100];
//	//swprintf_s(buff, L"Skel's Health : %3f", m_EnemyPTR->GetCom<SGAMECOMBAT>()->MonsterHealth());
//
//	//GameDebugEn::Draw2DFont(buff, 20.0f, { 0.0f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
//}
//
