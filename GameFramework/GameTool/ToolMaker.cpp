#include "pch.h"
#include "ToolMaker.h"
#include <LogicHeader.h>
#include <GameInput.h>
#include <GameActor.h>
#include <GameTransform.h>
#include <GameRenderer.h>
#include <GameCam.h>
#include <GameDic.h>
#include <GameImage.h>
#include <GameTexture.h>
#include <GameSprite.h>
#include <GameAni.h>
#include <GameDebugEn.h>
#include <GameTile.h>
#include <GameMiniMap.h>



SPtr<GameTile> ToolMaker::m_TileRender;
SPtr<GameMiniMap> ToolMaker::m_MinimapRender;

ToolMaker::ToolMaker() : m_Timer(0)
{
}


ToolMaker::~ToolMaker()
{
}


void ToolMaker::Loading()
{
	GameInput::CreateKey(L"MML", 'A');
	GameInput::CreateKey(L"MMR", 'D');
	GameInput::CreateKey(L"MMU", 'W');
	GameInput::CreateKey(L"MMD", 'S');
	GameInput::CreateKey(L"TILECREATE", VK_SPACE);
	GameInput::CreateKey(L"MouseRight", VK_RBUTTON);
	
	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	m_Dir.Move(L"res");
	
	if (!m_Dir.TryFindFileRecursive(L"Tile"))
	{
		ASSERTMSG;
	}

	GameTexture::Load(m_Dir.CreateGameFilePath(L"ShovelKnight.png"));
	GameTexture::Load(m_Dir.CreateGameFilePath(L"Tile1.png"));
	GameSprite::Create(m_Dir.CreateGameFilePath(L"ShovelKnight.png"), 5, 3);
	GameSprite::Create(m_Dir.CreateGameFilePath(L"Tile1.png"), 7, 7);

	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		m_Dir.Move(L"res");
		
		if (!m_Dir.TryFindFileRecursive(L"panels"))
		{
			ASSERTMSG;
		}

		if (nullptr == GameTexture::Find(L"panel_map.png"))
		{
			GameTexture::Load(m_Dir.CreateGameFilePath(L"panel_map.png"));
			GameSprite::Create(m_Dir.CreateGameFilePath(L"panel_map.png"), 1, 1);
		}

		if (!m_Dir.TryFindFileRecursive(L"icons_map"))
		{
			ASSERTMSG;
		}

		GameTexture::Load(m_Dir.CreateGameFilePath(L"hall_clear.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"hall_clear.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"hall_dark.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"hall_dark.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"hall_dim.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"hall_dim.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"hall_door.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"hall_door.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_battle.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_battle.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_boss.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_boss.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_curio.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_curio.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_empty.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_empty.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_entrance.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_entrance.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_treasure.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_treasure.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"room_unknown.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"room_unknown.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"indicator.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"indicator.png"), 1, 1);
	}


	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->LPOS({ 0, 0, -10.0f });
		m_Cam = PTR->AddCom<GameCam>(10, (unsigned int)RENDERGROUP::RG_MAP, (unsigned int)RENDERGROUP::RG_PLAYER, (unsigned int)RENDERGROUP::RG_MONSTER);

		m_Cam->Size(LogicHeader::ScreenSize);
	}

	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->LPOS({ 0, 0, -10.0f });
		UICam = PTR->AddCom<GameCam>(11, (unsigned int)RENDERGROUP::RG_UI);

		UICam->Size(LogicHeader::ScreenSize);
	}


	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->WPOS({ 0.0F, 0.0F, -10.0F });
		PTR->Trans()->WSCALE({ 1.0f,1.0f,1.0f });
		m_TileRender = PTR->AddCom<GameTile>();// 플레이어 그림 띄워주는 랜더러
		m_TileRender->Sprite(L"Tile1.png");
	}

	{
		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->WPOS({ 0.0F, 0.0F, 10.0F });
		PTR->Trans()->WSCALE(LogicHeader::ScreenSize);
		SPtr<GameImage> UI = PTR->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
		UI->Sprite(L"panel_map.png");
	}

	{

		SPtr<GameActor> PTR = CreateActor();
		PTR->Trans()->WPOS({ 0.0f,0.0f, 1.0f, 0.0f });
		PTR->Trans()->WSCALE({ 0.5f, 0.5f, 0.0f, 1.0f });

		m_MinimapRender = PTR->AddCom<GameMiniMap>((unsigned int)RENDERGROUP::RG_MAP);
		m_MinimapRender->ChangeMinmapRect(
			GameRect
			{ 	{-100.4f, 300.6f, 0.0f, 1.0f}
				, {600.4f, -300.6f, 0.0f, 1.0f} 
			});
	}


}

void ToolMaker::Update()
{
	if (true == GameInput::Press(L"MML"))
	{
		m_Cam->Trans()->LMOVETOTIMESPEED(CVector::LEFT, 10.0f);
	}

	if (true == GameInput::Press(L"MMR"))
	{
		m_Cam->Trans()->LMOVETOTIMESPEED(CVector::RIGHT, 10.0f);
	}
	
	if (true == GameInput::Press(L"MMU"))
	{
		m_Cam->Trans()->LMOVETOTIMESPEED(CVector::UP, 10.0f);
	}
	
	if (true == GameInput::Press(L"MMD"))
	{
		m_Cam->Trans()->LMOVETOTIMESPEED(CVector::DOWN, 10.0f);
	}

	if (true == GameInput::Down(L"TILECREATE"))
	{
		m_MinimapRender->MakeRandomMap(5);
	}

}

void ToolMaker::DebugRender()
{
	wchar_t Arr[256];
	swprintf_s(Arr, L"MousePos %3f, %3f", GameWin::MainWindow()->MousePos().x
		, GameWin::MainWindow()->MousePos().y);
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 80 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(Arr, L"MousePos3D %3f, %3f", GameWin::MainWindow()->MousePos3D().x
		, GameWin::MainWindow()->MousePos3D().y);
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 100 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(Arr, L"CamPos %3f, %3f", m_Cam->Trans()->WPOS().x
		, m_Cam->Trans()->WPOS().y);
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 120 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(Arr, L"CamScreenSize %3f, %3f"
		, m_Cam->Size().x
		, m_Cam->Size().y);
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 140 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(Arr, L"ScreenToMousePos %3f, %3f"
		, m_Cam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).x
		, m_Cam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).y);
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 160 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(Arr, L"Way Value Tex %d", (int)m_MinimapRender->m_AllRoomVec.size());
	GameDebugEn::Draw2DFont(Arr, 20.0f, { 100, 180 }, { 1.0f, 1.0f, 1.0f, 1.0f });


}


