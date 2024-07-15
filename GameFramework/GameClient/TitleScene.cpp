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
//3D수업 시작
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
		// 퍼스펙티브 일때는 원근감을 표현하기 때문에
		// 이제는 보이게 된다.
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
		SPtr<GameActor> PTR = CreateActor(L"플레이어");
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
//		//GAMECOMBAT을 받아놓고 턴 계산을 해주는 전투 매니저
//		//해당 액터의 FSM도 처리 가능. 근데 FSM은 어떻게 처리해야 하지?->저번에 한번 보여줬다.
//		//어차피 액터들 FSM은 공통적인 부분이 많은데 공통적인 FSM처리 컴포넌트를 만들고 싶다.
//		//UI 버튼 입력 -> FSM처리 컴포넌트와 전투매니저와 카메라에 전달
//		//FSM처리 컴포넌트는 동작 변경
//		//플레이어 클래스를 따로 만들어야 하나? 액터가 정보를 다 들고 있는데?
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
