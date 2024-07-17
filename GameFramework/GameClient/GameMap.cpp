#include "GameMap.h"
#include <GameDic.h>
#include <GameSprite.h>
#include <GameRef.h>
#include <GameComponent.h>
#include <GameActor.h>
#include <GameScene.h>
#include <GameRenderMgr.h>
#include <GameImage.h>
#include <LogicHeader.h>
#include <GameTime.h>
#include <GameInput.h>
#include <GameCam.h>

// directxtex.lib
GameMap::GameMap() : 
	m_FadeAlpha({ 1.0f, 1.0f, 1.0f, 0.0f })
	, m_FadeTimer(0)
	, m_HalfFadeTimer(0)
	, m_State(MapState::Room)
{

}


GameMap::~GameMap()
{
	if (nullptr != m_DungeonSprite)
	{
		delete m_DungeonSprite;
	}

	if (0 != m_BackRoadRender.size())
	{
		for (auto& BackRender : m_BackRoadRender)
		{
			delete BackRender;
			BackRender = nullptr;
		}

		m_BackRoadRender.clear();
	}
}


GameMap::DungeonSprite::DungeonSprite()
{

}


GameMap::DungeonSprite::~DungeonSprite()
{

}

void GameMap::Init()
{
	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	m_UIRender[0] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[1] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[2] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[3] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[4] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[5] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[6] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PreMapUI);
	m_UIRender[7] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[8] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[9] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[10] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
	m_UIRender[11] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);

	m_RoomRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP);
	m_RoomRender->WSCALE(LogicHeader::UpScreenSize);
	m_RoomRender->WPOS(LogicHeader::UpScreenPos);
	m_RoomRender->Off();
	float Padding = 4.2f;
	CVector tempPos;
	for (int i = 0; i < 9; i++)
	{
		DungeonWallBackRender* BackRoadRender = new DungeonWallBackRender();
		BackRoadRender->m_Corridor_back = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_BackWall);
		BackRoadRender->m_Corridor_back->WSCALE(LogicHeader::CorridorSize);
		CVector tempPos = LogicHeader::CorridorPos;
		tempPos.x += i * Padding;
		BackRoadRender->m_Corridor_back->WPOS(tempPos);
		BackRoadRender->m_Corridor_back->Off();


		BackRoadRender->m_Corridor_mid = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MidWall);
		BackRoadRender->m_Corridor_mid->WSCALE(LogicHeader::CorridorSize);
		tempPos = LogicHeader::CorridorPos;
		tempPos.x += i * Padding;
		tempPos.z -= 2.0f;
		tempPos.y += 0.6f;
		BackRoadRender->m_Corridor_mid->WPOS(tempPos);
		BackRoadRender->m_Corridor_mid->Off();

		BackRoadRender->m_WallRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP);
		BackRoadRender->m_WallRender->WSCALE(LogicHeader::CorridorSize);
		tempPos = LogicHeader::CorridorPos;
		tempPos.x += i * Padding;
		tempPos.z -= 3.0f;
		BackRoadRender->m_WallRender->WPOS(tempPos);
		BackRoadRender->m_WallRender->Off();
		m_BackRoadRender.push_back(BackRoadRender);

		if (i == 1)
		{
			m_StartDoor = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP2);
			tempPos.z -= 0.1f;
			tempPos.x -= 0.08f;
			tempPos.y += 0.15f;
			m_StartDoor->WPOS(tempPos);
			m_StartDoor->WSCALE(LogicHeader::DoorSize);
			m_StartDoor->Off();
			m_StartDoorPos = tempPos;
		}

		if (i == 7)
		{
			m_EndDoor = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP2);
			tempPos.z -= 0.1f;
			tempPos.x -= 0.08f;
			tempPos.y += 0.15f;
			m_EndDoor->WPOS(tempPos);
			m_EndDoor->WSCALE(LogicHeader::DoorSize);
			m_EndDoor->Off();
			m_EndDoorPos = tempPos;
		}
	}

	//endhall 이미지를 위해서 따로 만듦
	DungeonWallBackRender* BackRoadRender = new DungeonWallBackRender();
	BackRoadRender->m_WallRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP);
	BackRoadRender->m_WallRender->WSCALE(LogicHeader::CorridorSize);
	tempPos = LogicHeader::CorridorPos;
	tempPos.x += (m_BackRoadRender.size() - 1) * Padding;
	tempPos.x -= 1.0f;
	tempPos.z -= 3.5f;
	BackRoadRender->m_WallRender->WPOS(tempPos);
	BackRoadRender->m_WallRender->Off();
	m_BackRoadRender.push_back(BackRoadRender);
}

bool GameMap::MapLoad(GameString _Name)
{
	Name(_Name);

	m_CorridorPart_Name[0] = SStringName() + L".corridor_bg";
	m_CorridorPart_Name[1] = SStringName() + L".corridor_door.basic";
	m_CorridorPart_Name[2] = SStringName() + L".corridor_mid";
	m_CorridorPart_Name[3] = SStringName() + L".endhall.01";
	m_CorridorPart_Name[4] = SStringName() + L".foreground_bottom.01";
	m_CorridorPart_Name[5] = SStringName() + L".foreground_top.01";

	m_DungeonSprite = new DungeonSprite();
	m_DungeonSprite->m_DoorSprite.reserve(2);

	GameDic Dir;
	Dir.MoveParent(L"GameFramework");

	if (Name() == L"")
	{
		ASSERTMSG;
		return false;
	}

	if (!Dir.TryFindFileRecursive(Name()))
	{
		ASSERTMSG;
		return false;
	}

	std::vector<GameFile> Img = Dir.DirToAllFile();

	for (auto& file : Img)
	{
		if (L".png" == file.Ext())
		{
			for (size_t i = 0; i < 7; ++i)
			{
				if (m_CorridorPart_Name[i] == file.CFileName())
				{
					GameTexture::Load(file.CFullPath());
					m_DungeonSprite->m_DefaultImg[i] = GameSprite::Create(file.CFullPath(), 1, 1);
					break;
				}
			}


			if (file.CFileName().find(L"corridor_wall") != std::wstring::npos)
			{
				GameTexture::Load(file.CFullPath());
				m_DungeonSprite->m_Corridor_Wall.insert(std::map<GameString, SPtr<GameSprite>>::value_type(file.CFileName(), (GameSprite::Create(file.CFullPath(), 1, 1))));
			}

			if (file.CFileName().find(L"room_wall") != std::wstring::npos)
			{
				GameTexture::Load(file.CFullPath());
				m_DungeonSprite->m_Room_Wall.insert(std::map<GameString, SPtr<GameSprite>>::value_type(file.CFileName(), GameSprite::Create(file.CFullPath(), 1, 1)));
			}

			if (file.CFileName().find(L"weald_door_close") != std::wstring::npos)
			{
				GameTexture::Load(file.CFullPath());
				m_DungeonSprite->m_DoorSprite.push_back(GameSprite::Create(file.CFullPath(), 1, 1));
			}

			if (file.CFileName().find(L"weald_door_open") != std::wstring::npos)
			{
				GameTexture::Load(file.CFullPath());
				m_DungeonSprite->m_DoorSprite.push_back(GameSprite::Create(file.CFullPath(), 1, 1));
			}
		}
	}

	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		m_Dir.Move(L"res");
		if (!m_Dir.TryFindFileRecursive(L"panels"))
		{
			ASSERTMSG;
		}
		GameTexture::Load(m_Dir.CreateGameFilePath(L"side_decor.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"side_decor.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"panel_banner_controller.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"panel_banner_controller.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"panel_hero.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"panel_hero.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"panel_map_upper_piece.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"panel_map_upper_piece.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"panel_map.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"panel_map.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"focuscam_button.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"focuscam_button.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"party_order_button.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"party_order_button.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"torch.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"torch.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"bottom_background.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"bottom_background.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"fadeCall.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"fadeCall.png"), 1, 1);
	}

	{
		m_UISprite[0] = GameSprite::Find(L"side_decor.png");
		m_UISprite[1] = GameSprite::Find(L"panel_banner_controller.png");
		m_UISprite[2] = GameSprite::Find(L"panel_hero.png");
		m_UISprite[3] = GameSprite::Find(L"panel_map.png");
		m_UISprite[4] = GameSprite::Find(L"focuscam_button.png");
		m_UISprite[5] = GameSprite::Find(L"party_order_button.png");
		m_UISprite[6] = GameSprite::Find(L"torch.png");
		m_UISprite[7] = GameSprite::Find(L"panel_map_upper_piece.png");
		m_UISprite[8] = GameSprite::Find(L"bottom_background.png");
		m_UISprite[9] = GameSprite::Find(L"fadeCall.png");
	}

	{
		m_DungeonSprite->m_Corridor_Wall_Iter = m_DungeonSprite->m_Corridor_Wall.begin();
		m_BackRoadRender[0]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[0]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[0]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		m_BackRoadRender[1]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[1]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[1]->m_WallRender->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_door]);
		m_StartDoor->Sprite(m_DungeonSprite->m_DoorSprite[0]);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[2]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[2]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[2]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[3]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[3]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[3]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[4]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[4]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[4]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[5]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[5]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[5]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[6]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[6]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[6]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		m_BackRoadRender[7]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[7]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[7]->m_WallRender->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_door]);
		m_EndDoor->Sprite(m_DungeonSprite->m_DoorSprite[0]);

		++m_DungeonSprite->m_Corridor_Wall_Iter;
		m_BackRoadRender[8]->m_Corridor_back->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_bg]);
		m_BackRoadRender[8]->m_Corridor_mid->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::corridor_mid]);
		m_BackRoadRender[8]->m_WallRender->Sprite(m_DungeonSprite->m_Corridor_Wall_Iter->second);

		m_BackRoadRender[9]->m_WallRender->Sprite(m_DungeonSprite->m_DefaultImg[DefaultImg::endhall]);
	}

	return true;
}

void GameMap::DisplayMapAndUI()
{
	Play_TutorialRoom(GameMiniMap::RoomSpriteName::room_entrance);

	m_UIRender[0]->Sprite(m_UISprite[UISpirte::side_decor]);
	m_UIRender[0]->WSCALE({ 1.62f, 2.5f, 0.0f , 1.0f });
	m_UIRender[0]->WPOS({ -5.59f, -2.4f, LogicHeader::MinimapZpos - 1.0f, 1.0f });

	m_UIRender[1]->Sprite(m_UISprite[UISpirte::panel_banner_controller]);
	m_UIRender[1]->WSCALE({ 4.78f, 0.90f, 0.0f , 1.0f });
	m_UIRender[1]->WPOS({ -2.39f, -1.64f, LogicHeader::MinimapZpos - 1.0f, 1.0f });

	m_UIRender[2]->Sprite(m_UISprite[UISpirte::panel_hero]);
	m_UIRender[2]->WSCALE({ 4.50f, 1.40f, 0.0f , 1.0f });
	m_UIRender[2]->WPOS({ -2.43f, -2.8f, LogicHeader::MinimapZpos - 1.0f, 1.0f });

	m_UIRender[3]->Sprite(m_UISprite[UISpirte::panel_map]);
	m_UIRender[3]->WSCALE({ 4.80f, 2.35f, 0.0f , 1.0f });
	m_UIRender[3]->WPOS({ 2.4f, -2.34f, LogicHeader::MinimapZpos + 1.0f, 1.0f });

	m_UIRender[4]->Sprite(m_UISprite[UISpirte::side_decor]);
	m_UIRender[4]->WSCALE({ -1.62f, 2.5f, 0.0f , 1.0f });
	m_UIRender[4]->WPOS({ 5.61f, -2.4f, LogicHeader::MinimapZpos - 1.0f, 1.0f });

	m_UIRender[10]->Sprite(m_UISprite[UISpirte::torch]);
	m_UIRender[10]->WSCALE({ -6.0f, 1.15f, 0.0f , 1.0f });
	m_UIRender[10]->WPOS({ 0.0f, 2.775f, LogicHeader::MinimapZpos - 1.0f, 1.0f });

	m_UIRender[6]->Sprite(m_UISprite[UISpirte::panel_map_upper_piece]);
	m_UIRender[6]->WSCALE({ 4.80f, 2.35f, 0.0f , 1.0f });
	m_UIRender[6]->WPOS({ 2.4f, -2.34f, LogicHeader::MinimapZpos - 1.1f, 1.0f });

	m_UIRender[7]->Sprite(m_UISprite[UISpirte::bottom_background]);
	m_UIRender[7]->WSCALE({ 6.4f, 2.68f });
	m_UIRender[7]->WPOS({ -3.2f, -2.35f , 9.0f, 1.0f });

	m_UIRender[8]->Sprite(m_UISprite[UISpirte::bottom_background]);
	m_UIRender[8]->WSCALE({ 6.4f, 0.28f });
	m_UIRender[8]->WPOS({ 3.2f, -1.15f, 9.0f, 1.0f });

	m_UIRender[9]->Sprite(m_UISprite[UISpirte::bottom_background]);
	m_UIRender[9]->WSCALE({ 6.4f, 0.15f });
	m_UIRender[9]->WPOS({ 3.2f, -3.58f, 9.0f, 1.0f });

	m_UIRender[5]->Sprite(m_UISprite[UISpirte::bottom_background]);
	m_UIRender[5]->WSCALE({ 12.8f, 1.0f });
	m_UIRender[5]->WPOS({ 0.0f, 3.2f, 9.5f, 1.0f });
	m_UIRender[5]->Off();

	m_UIRender[11]->Sprite(m_UISprite[UISpirte::focuscam_button]);
	m_UIRender[11]->WSCALE({ 0.32f, 0.4f });
	m_UIRender[11]->WPOS({ 4.65f, -1.5f, 8.9f, 1.0f });
}


void GameMap::FadeCall(SPtr<GameCam> _Cam, float _Duration /*= 0.5f*/)
{
	m_FadeTimer = _Duration;
	m_HalfFadeTimer = m_FadeTimer / 2;
	CVector tempPos = _Cam->Trans()->WPOS();
	tempPos.y += 1.4f;

	if (nullptr == m_FadeImgRender)
	{
		m_FadeImgRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MAP2);
		m_FadeImgRender->Sprite(m_UISprite[UISpirte::fadeCall]);
		CVector tempVec = LogicHeader::ScreenSize;
		tempVec.y -= 2.35f;
		m_FadeImgRender->WSCALE(tempVec);
		m_FadeImgRender->WPOS(tempPos);
	}
	else
	{
		m_FadeAlpha = { 1.0f, 1.0f, 1.0f, 0.0f };
		m_FadeImgRender->On();
		m_FadeImgRender->WPOS(tempPos);
	}

}

void GameMap::ChangeFadePos(CVector _Pos)
{
	m_FadeImgRender->WPOS(_Pos);
}

void GameMap::UpdatePrev()
{
}
void GameMap::Update()
{
	if (m_FadeTimer > 0)
	{
		m_FadeTimer -= GameTime::DeltaTime();
	}

	if (nullptr != m_FadeImgRender)
	{
		if (m_FadeTimer > m_HalfFadeTimer)
		{
			m_FadeAlpha.w += GameTime::DeltaTime() * 2;
			m_FadeImgRender->Color(m_FadeAlpha);
		}
		else if (m_FadeTimer > 0.0f)
		{
			m_FadeAlpha.w -= GameTime::DeltaTime() * 2;
			m_FadeImgRender->Color(m_FadeAlpha);

			if (m_State == Room)
			{
				m_UIRender[5]->Off();
			}
			else
			{
				m_UIRender[5]->On();
			}
		}
	}

	if (m_FadeTimer < 0.0f && nullptr != m_FadeImgRender)
	{
		m_FadeImgRender->Off();
		CloseDoor();
	}


}
void GameMap::UpdateNext()
{
}

void GameMap::DebugRender()
{
}

void GameMap::Play_Road_Render()
{
	AllRenderOff();
	m_State = MapState::Road;

	for (int i = 0; i < (int)m_BackRoadRender.size(); i++)
	{
		m_BackRoadRender[i]->RenderOn();
	}

	m_StartDoor->On();
	m_EndDoor->On();
}

void GameMap::Play_CryptRoom_Render(GameMiniMap::RoomSpriteName _RoomName /*= GameMiniMap::RoomSpriteName::RoomMax*/)
{
	AllRenderOff();
	m_State = MapState::Room;

	GameString RoomName = SStringName();
	switch (_RoomName)
	{
	case GameMiniMap::RoomSpriteName::room_battle:
		break;
	case GameMiniMap::RoomSpriteName::room_boss:
		break;
	case GameMiniMap::RoomSpriteName::room_curio:
		break;
	case GameMiniMap::RoomSpriteName::room_empty:
		break;
	case GameMiniMap::RoomSpriteName::room_entrance:
		RoomName += L".entrance_room_wall";
		m_DungeonSprite->m_Room_Iter = m_DungeonSprite->m_Room_Wall.find(RoomName);
		if (m_DungeonSprite->m_Room_Wall.end() == m_DungeonSprite->m_Room_Iter)
		{
			ASSERTMSG;
		}
		m_RoomRender->Sprite(m_DungeonSprite->m_Room_Iter->second);
		m_RoomRender->On();
		break;
	case GameMiniMap::RoomSpriteName::room_treasure:
		break;
	case GameMiniMap::RoomSpriteName::RoomMax:
	default:
		ASSERTMSG;
		break;
	}
}

void GameMap::Play_TutorialRoom(GameMiniMap::RoomSpriteName _RoomName /*= GameMiniMap::RoomSpriteName::RoomMax*/)
{
	AllRenderOff();
	m_State = MapState::Room;

	GameString RoomName = SStringName();
	switch (_RoomName)
	{
	case GameMiniMap::RoomSpriteName::room_entrance:
		RoomName = L"weald.tutorial.room_wall.effigy_0";
		m_DungeonSprite->m_Room_Iter = m_DungeonSprite->m_Room_Wall.find(RoomName);
		if (m_DungeonSprite->m_Room_Wall.end() == m_DungeonSprite->m_Room_Iter)
		{
			ASSERTMSG;
		}
		m_RoomRender->Sprite(m_DungeonSprite->m_Room_Iter->second);
		m_RoomRender->On();
		break;
	default:
		RoomName = L"weald.tutorial.room_wall.effigy_1";
		m_DungeonSprite->m_Room_Iter = m_DungeonSprite->m_Room_Wall.find(RoomName);
		if (m_DungeonSprite->m_Room_Wall.end() == m_DungeonSprite->m_Room_Iter)
		{
			ASSERTMSG;
		}
		m_RoomRender->Sprite(m_DungeonSprite->m_Room_Iter->second);
		m_RoomRender->On();
		break;
	}
}

void GameMap::AllRenderOff()
{
	m_RoomRender->Off();
	m_StartDoor->Off();
	m_EndDoor->Off();

	for (int i = 0; i < (int)m_BackRoadRender.size(); i++)
	{
		m_BackRoadRender[i]->RenderOff();
	}
}

void GameMap::OpenDoor(bool _is_StartDoor)
{
	if (true == _is_StartDoor)
	{
		m_StartDoor->Sprite(m_DungeonSprite->m_DoorSprite[1]);
	}
	else
	{
		m_EndDoor->Sprite(m_DungeonSprite->m_DoorSprite[1]);
	}
}

void GameMap::CloseDoor()
{
	m_StartDoor->Sprite(m_DungeonSprite->m_DoorSprite[0]);
	m_EndDoor->Sprite(m_DungeonSprite->m_DoorSprite[0]);
}