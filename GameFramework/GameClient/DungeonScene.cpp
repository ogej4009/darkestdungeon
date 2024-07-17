#include <GameDic.h>
#include <GameTexture.h>
#include <GameSprite.h>
#include <GameAni.h>
#include <GameImage.h>
#include <GameCam.h>
#include <GameDebugEn.h>
#include <GameInput.h>

#include "Combat.h"
#include "TextReader.h"
#include "GameJsonReader.h"
#include "GameMap.h"
#include "DungeonScene.h"
#include "HeroFsm.h"
#include "MonsterFsm.h"
#include "TargetRender.h"


DungeonScene::DungeonScene() : m_CanMove(true), m_Is_Battle(false)
{
	m_PlayerGroup.reserve(4);
	//로직헤더에 던전별 몬스터 정보를 추가해 enumclass를 만들고 해당 max사이즈로 리저브
	m_MonsterGroup.reserve(4);
}

DungeonScene::~DungeonScene()
{

}

void DungeonScene::Loading()
{
	//던전을 불러오기 전에 InitCom을 호출하는 친구가 로딩화면 출력을 대신 해줘야 한다. 지금은 임시로 여기서 호출하는 것
	{
		GameInput::CreateKey(L"Space", VK_SPACE);
		GameInput::CreateKey(L"FadeCall", L'R');
		GameInput::CreateKey(L"FadeCall2", L'r');
		GameInput::CreateKey(L"MML", 'A');
		GameInput::CreateKey(L"MMR", 'D');
		GameInput::CreateKey(L"MMU", 'W');
		GameInput::CreateKey(L"MMD", 'S');
		GameInput::CreateKey(L"MinimapCamToOrigin", 'G');
		GameInput::CreateKey(L"LeftMouseClick", VK_LBUTTON);
		GameInput::CreateKey(L"RightMouseClick", VK_RBUTTON);
		GameInput::CreateKey(L"Up", L'W');
		GameInput::CreateKey(L"UpKey", VK_UP);
		GameInput::CreateKey(L"Hero1", VK_F1);
		GameInput::CreateKey(L"Hero2", VK_F2);
		GameInput::CreateKey(L"Hero3", VK_F3);
		GameInput::CreateKey(L"Hero4", VK_F4);
		GameInput::CreateKey(L"Skill1", '1');
		GameInput::CreateKey(L"Skill2", '2');
		GameInput::CreateKey(L"Skill3", '3');
		GameInput::CreateKey(L"Skill4", '4');
		GameInput::CreateKey(L"MonsterSkill0", '7');
		GameInput::CreateKey(L"MonsterSkill1", '8');
		GameInput::CreateKey(L"MonsterSkill2", '9');
		GameInput::CreateKey(L"MonsterSkill3", '0');
		GameInput::CreateKey(L"TargetSkill1", VK_NUMPAD1);
		GameInput::CreateKey(L"TargetSkill2", VK_NUMPAD2);
		GameInput::CreateKey(L"TargetSkill3", VK_NUMPAD3);
		GameInput::CreateKey(L"TargetSkill4", VK_NUMPAD4);
		GameInput::CreateKey(L"TargetSkill5", VK_NUMPAD5);
		GameInput::CreateKey(L"TargetSkill6", VK_NUMPAD6);
		GameInput::CreateKey(L"TargetSkill7", VK_NUMPAD7);

	}

	{
		m_LoadingImage = CreateActor(L"LoadingScene");
		m_LoadingImage->Trans()->LPOS({ 0.0f, 0.0f, -7.9f });
		m_LoadingImage->Trans()->LSCALE(LogicHeader::ScreenSize);
		SPtr<GameImage> m_UIRender = m_LoadingImage->AddCom<GameImage>(0);

		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		m_Dir.Move(L"res");
		//m_Dir.Move(L"Tex");
		//m_Dir.Move(L"loading_screen");

		// C:\repo\darkestdungeon\GameFramework\res\Tex\loading_screen
		if (!m_Dir.TryFindFileRecursive(L"loading_screen"))
		{
			ASSERTMSG;
		}
		GameTexture::Load(m_Dir.CreateGameFilePath(L"loading_screen.crypts_0.png"));
		SPtr<GameSprite> tempSprite = GameSprite::Create(m_Dir.CreateGameFilePath(L"loading_screen.crypts_0.png"), 1, 1);
		m_UIRender->Sprite(tempSprite);
	}

	{
		SPtr<GameActor> PTR = CreateActor(L"MAINCAM");
		PTR->Trans()->LPOS({ 0.0f, 0.0f, 0.0f });
		m_MainCam = PTR->AddCom<GameCam>(9, (unsigned int)RENDERGROUP::RG_MAP, (unsigned int)RENDERGROUP::RG_MAP2, (unsigned int)RENDERGROUP::RG_PLAYER, (unsigned int)RENDERGROUP::RG_MONSTER, (unsigned int)RENDERGROUP::RG_TARGET);
		m_MainCam->Size(LogicHeader::ScreenSize);
	}

	{
		SPtr<GameActor> PTR = CreateActor(L"BackCAM");
		PTR->Trans()->LPOS({ 0.0f, 0.0f, 0.0f });
		m_BackCam = PTR->AddCom<GameCam>(0, (unsigned int)RENDERGROUP::RG_BackWall);
		m_BackCam->Size(LogicHeader::ScreenSize);
	}

	{
		SPtr<GameActor> PTR = CreateActor(L"MIDCAM");
		PTR->Trans()->LPOS({ 0.0f, 0.0f, 0.0f });
		m_MidCam = PTR->AddCom<GameCam>(1, (unsigned int)RENDERGROUP::RG_MidWall);
		m_MidCam->Size(LogicHeader::ScreenSize);
	}

	{
		SPtr<GameActor> PTR = CreateActor(L"UICAM");
		PTR->Trans()->LPOS({ 0.0f, 0.0f, 0.0f });
		m_UICam = PTR->AddCom<GameCam>(11, (unsigned int)RENDERGROUP::RG_UI, (unsigned int)RENDERGROUP::RG_PreMapUI, (unsigned int)RENDERGROUP::RG_PostUI);
		m_UICam->Size(LogicHeader::ScreenSize);
	}

	{
		for (int i = 0; i < 2; i++)
		{
			HeroTargetInit(ActorList::PlayerList[i], i);
		}
		GameString map = L"weald";
		GameString temp = L"skeleton_common";
		MonsterInit(map, temp, 0, 3);
		DungeonInit(map);
		map = L"common";
		temp = L"brigand_blood";
		MonsterInit(map, temp, 0, 0, 1);
		temp = L"brigand_cutthroat";
		MonsterInit(map, temp, 0, 2, 1);
		temp = L"brigand_fusilier";
		MonsterInit(map, temp, 0, 1, 1);
		MinimapInit();
		CombatInit();
	}

	for (auto& Monster : m_MonsterGroup)
	{
		Monster->Off();
	}

	m_AState = ASTATE::MOVEROOM;
	m_CurUnitPointer; //GAMECOMBAT에서 pointing 관리해줘야 함

	//튜토리얼 만드는 용도, 이후엔 randommap으로 생성해야함
	{
		int Arr[] = { 2 };
		MakeOrderMap(1, Arr, 1);
		GameMiniMap::SKEY tempKey = GameMiniMap::SKEY(0, 0);
		m_CurRoomIter = m_MinimapRender->m_AllRoom.find(tempKey.TileKey);
	}
}

void DungeonScene::MinimapInit()
{
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
		GameTexture::Load(m_Dir.CreateGameFilePath(L"moving_room.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"moving_room.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"indicator.png"));
		GameSprite::Create(m_Dir.CreateGameFilePath(L"indicator.png"), 1, 1);
	}

	SPtr<GameActor> PTR = CreateActor();
	PTR->Trans()->WPOS({ 2.4f, -2.5f, LogicHeader::MinimapZpos });
	PTR->Trans()->WSCALE({ 0.2f,0.2f,0.2f });

	//Render의 Init은 DrawOrder관리에 들어간다.
	m_MinimapRender = PTR->AddCom<GameMiniMap>((unsigned int)RENDERGROUP::RG_UI);
}

void DungeonScene::Update()
{
	m_LoadingImage->Death();
	// 여기서 fsm을 만든다.
	switch (m_AState)
	{
	case DungeonScene::ASTATE::BATTLE:
		BattleState();
		break;
	case DungeonScene::ASTATE::MOVEROOM:
		MoveRoomState();
		break;
	case DungeonScene::ASTATE::MOVEROAD:
		MoveRoadState();
		break;
	case DungeonScene::ASTATE::EVENT:
		break;
	default:
		break;
	}

	m_MainCamPos = m_MainCam->Trans()->WPOS();
	m_FPSTimer += GameTime::DeltaTime();
	m_BattleTimer -= GameTime::DeltaTime();

	RearrageCamAndFade();
	RearrangeMinimapFocusCam();
	CheckIndex();
	MoveMinimap();
	CheckHeroCanMove();
	CheckCombat();


	if (m_FPSTimer - 1.0f > 0)
	{
		m_FPSTimer = 0;
		m_FPS = 1 / GameTime::DeltaTime();
		m_FPSstr = L"FPS : " + std::to_wstring((int)m_FPS);
	}

	if (true == GameInput::Down(L"MinimapCamToOrigin"))
	{
		float x = m_MinimapRender->Trans()->WPOS().x + 2.2f - m_MinimapRender->m_MinimapIndicatorPos.x;
		float y = m_MinimapRender->Trans()->WPOS().y + -2.2f - m_MinimapRender->m_MinimapIndicatorPos.y;
		m_MinimapRender->Trans()->WPOS({ x, y, 10.0f, 1.0f });
	}


	if (m_TargetPos.size() < 4)
	{
		m_TargetPos.push_back(true);
		m_TargetPos.push_back(true);
		m_TargetPos.push_back(false);
		m_TargetPos.push_back(false);
	}

	if (GameInput::Down(L"TargetSkill1") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(0, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill2") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(1, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill3") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(2, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill4") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(3, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill5") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(4, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill6") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(5, m_MainCam, m_TargetPos);
	}

	if (GameInput::Down(L"TargetSkill7") && true == b_Is_Hero)
	{
		m_CurTargetRender->TargetRenderOn(6, m_MainCam, m_TargetPos);
	}
}

//전투관련 체크 모음
void DungeonScene::CheckCombat()
{
	if (m_Is_Battle == false)
	{
		m_GameCombat->ChangeSelectUI(m_PlayerGroup[0]->Trans()->WPOS() - m_MainCam->Trans()->WPOS() + LogicHeader::SelectUIPlayerXOffset);
	}

	if (true == m_GameCombat->CheckCombat(m_MinimapRender->m_MinimapIndicator->m_Key.X, m_MinimapRender->m_MinimapIndicator->m_Key.Y) && m_Is_Battle == false)
	{
		ChangeState(ASTATE::BATTLE);
		m_CurCombat = m_GameCombat->m_CurCombat->second;
		for (auto& MonsterActor : m_CurCombat->m_MonsterActor)
		{
			MonsterActor->On();
			CVector temp = MonsterActor->Trans()->WPOS();
			temp += m_MainCam->Trans()->WPOS();
			MonsterActor->Trans()->WPOS(temp);
		}
		m_GameCombat->StartBattle(m_MainCam);

		HeroFsm* tempFSM;
		for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
		{
			tempFSM = m_PlayerGroup[i]->GetCom<HeroFsm>();
			tempFSM->BattleStart();
			tempFSM->ChangeFSM(HeroFSMLIST::Combat);
		}

		m_GameCombat->b_Is_Battle = true;
	}
	else if (false == m_Is_Battle)
	{
		m_GameCombat->b_Is_Battle = false;
	}

	CheckHeroIndex();
	CheckHeroSkill();
	CheckMonsterSkill();
}

void DungeonScene::CheckHeroIndex()
{
	//combat 관련 입력 체크
	if (true == GameInput::Down(L"Hero1") && nullptr != m_PlayerGroup[0])
	{
		m_GameCombat->m_HeroIndex = 0;
	}

	if (true == GameInput::Down(L"Hero2") && nullptr != m_PlayerGroup[1])
	{
		m_GameCombat->m_HeroIndex = 1;
	}

	if (true == GameInput::Down(L"Hero3") && nullptr != m_PlayerGroup[2])
	{
		m_GameCombat->m_HeroIndex = 2;
	}

	if (true == GameInput::Down(L"Hero4") && nullptr != m_PlayerGroup[3])
	{
		m_GameCombat->m_HeroIndex = 3;
	}
}

void DungeonScene::CheckMonsterSkill()
{
	if (true == b_Is_Hero || nullptr == m_CurCombatActor)
	{
		return;
	}

	int size = m_CurMonsterCombat->SkillCount();
	int targetPos = 0;

	for (int i = 0; i < size; i++)
	{
		if (true == m_GameCombat->CheckSkillLaunch(*m_CurCombatActor, i))
		{
			//calcdamage 하고 바로 끝
			targetPos = (m_GameCombat->CheckMonsterSkillTarget())[0];
			m_GameCombat->CalcHeroDamge(m_GameCombat->m_CurMonsterSkill, targetPos);
			m_BattleTimer = -1.0f;
			break;
		};

		//0.5초정도 대기하게 한 다음에 fsm바꾸고
	}


	m_CurMonsterFSM->ChangeFSM(MonsterFSMLIST::Skill1, false);
}

void DungeonScene::CheckHeroSkill()
{
	if (false == b_Is_Hero)
	{
		return;
	}

	if (true == GameInput::Down(L"Skill1") && m_GameCombat->b_Can_Activate_Skill[0] == true)
	{
		m_GameCombat->m_SelectSkillIndex = 0;
		m_GameCombat->m_SelectAbilitypriteRender->Color({ 1.0f, 1.0f, 1.0f, 0.0f });
		//게임컴뱃에서 타겟 측정하는 기능을 구현해 줘야됨
		//SGAMECOMBAT->CheckSkillTarget(m_CurHeroActor, SkillNum);

		m_GameCombat->CheckSkillLaunch(*m_CurCombatActor, 0, 0, true);
		m_GameCombat->CheckSkillTarget(&m_TargetPos);
		b_Can_Activate_Skill = true;
		m_DUNGEON_NextFSM = HeroFSMLIST::Skill1;
		m_CurSkillCount = 0;
	}

	if (true == GameInput::Down(L"Skill2") && m_GameCombat->b_Can_Activate_Skill[1] == true)
	{
		m_GameCombat->m_SelectSkillIndex = 1;
		m_GameCombat->m_SelectAbilitypriteRender->Color({ 1.0f, 1.0f, 1.0f, 0.0f });

		m_GameCombat->CheckSkillLaunch(*m_CurCombatActor, 1, 0, true);
		m_GameCombat->CheckSkillTarget(&m_TargetPos);
		b_Can_Activate_Skill = true;
		m_DUNGEON_NextFSM = HeroFSMLIST::Skill2;
		m_CurSkillCount = 1;
	}

	if (true == GameInput::Down(L"Skill3") && m_GameCombat->b_Can_Activate_Skill[2] == true)
	{
		m_GameCombat->m_SelectSkillIndex = 2;
		m_GameCombat->m_SelectAbilitypriteRender->Color({ 1.0f, 1.0f, 1.0f, 0.0f });

		m_GameCombat->CheckSkillLaunch(*m_CurCombatActor, 2, 0, true);
		m_GameCombat->CheckSkillTarget(&m_TargetPos);
		b_Can_Activate_Skill = true;
		m_DUNGEON_NextFSM = HeroFSMLIST::Skill3;
		m_CurSkillCount = 2;
	}

	if (true == GameInput::Down(L"Skill4") && m_GameCombat->b_Can_Activate_Skill[3] == true)
	{
		m_GameCombat->m_SelectSkillIndex = 3;
		m_GameCombat->m_SelectAbilitypriteRender->Color({ 1.0f, 1.0f, 1.0f, 0.0f });

		m_GameCombat->CheckSkillLaunch(*m_CurCombatActor, 3, 0, true);
		m_GameCombat->CheckSkillTarget(&m_TargetPos);
		b_Can_Activate_Skill = true;
		m_DUNGEON_NextFSM = HeroFSMLIST::Skill4;
		m_CurSkillCount = 3;
	}

	if (true == b_Can_Activate_Skill)
	{
		for (int i = 0; i < 4; i++)
		{
			if (true == GameInput::Down(L"LeftMouseClick") && true == CheckRectRange(LogicHeader::TargetRange[i], false))
			{
				m_GameCombat->CalcDamge(i);
				m_CurHeroFSM->ChangeFSM(m_DUNGEON_NextFSM, false);
				b_Hero_Phase_End = true;
				m_GameCombat->CloseTarget();
				b_Can_Activate_Skill = false;
				m_CurTargetRender->TargetRenderOn(m_CurSkillCount, m_MainCam, m_TargetPos);
			}
		}
	}

}

void DungeonScene::CombatInit()
{
	SPtr<GameActor> CombatActor = CreateActor(L"CombatActor");
	CombatActor->Trans()->WPOS({ 0.0f, 0.0f, 0.0f, 1.0f });
	m_GameCombat = CombatActor->AddCom<SGAMECOMBAT>();
	m_GameCombat->ChangeSelectUI(m_PlayerGroup[0]->Trans()->WPOS());

	SPtr<SHEROCOMBAT> tempHeroCombat;
	int index = 0;

	for (auto& Hero : m_PlayerGroup)
	{
		m_GameCombat->m_HeroActor.push_back(Hero);
		tempHeroCombat = Hero->GetCom<SHEROCOMBAT>();
		tempHeroCombat->m_CombatPosition = index;
		++index;
		m_GameCombat->m_HeroData.push_back(tempHeroCombat);
	}

	GameMiniMap::SKEY tempKey = GameMiniMap::SKEY(2, 0);
	std::vector<SPtr<GameActor>> tempActorVec;
	tempActorVec.push_back(m_MonsterGroup[0]);
	tempActorVec.push_back(m_MonsterGroup[1]);
	m_GameCombat->CreateCombat(tempKey, tempActorVec);
}

void DungeonScene::RearrageCamAndFade()
{
	if (m_GameMap->FadeTimer() > 0.0f && m_GameMap->FadeTimer() < m_GameMap->FadeHalfTimer())
	{
		CVector Origin = CVector({ 0.0f, 0.0f, 0.0f, 1.0f });
		m_MainCam->Trans()->WPOS(Origin);
		m_MidCam->Trans()->WPOS(Origin);
		Origin.y += 1.4f;
		m_GameMap->ChangeFadePos(Origin);
		CVector tempVec = LogicHeader::PlayerStartPos;
		for (int i = 0; i < (int)m_PlayerGroup.size(); ++i)
		{
			m_PlayerGroup[i]->Trans()->WPOS({ tempVec.x - i * 1.1f,tempVec.y, tempVec.z - i * 0.01f, tempVec.w });
		}
	}
}

void DungeonScene::MoveMinimap()
{
	CVector CurMousePos = CVector({ GameWin::MainWindow()->MousePos3D().x,GameWin::MainWindow()->MousePos3D().y, 1.0f, 1.0f });

	if (true == CheckRectRange(GameMiniMap::MinimapRange) && true == GameInput::Press(L"RightMouseClick"))
	{

		CVector calcMousePos = m_PrevMousePos - CurMousePos;

		if (calcMousePos.x < 0)
		{
			m_MinimapRender->Trans()->LMOVETOTIMESPEED(CVector::LEFT, 5.0f);
		}
		if (calcMousePos.x > 0)
		{
			m_MinimapRender->Trans()->LMOVETOTIMESPEED(CVector::RIGHT, 5.0f);
		}
		if (calcMousePos.y < 0)
		{
			m_MinimapRender->Trans()->LMOVETOTIMESPEED(CVector::UP, 5.0f);
		}
		if (calcMousePos.y > 0)
		{
			m_MinimapRender->Trans()->LMOVETOTIMESPEED(CVector::DOWN, 5.0f);
		}
	}

	m_PrevMousePos = CurMousePos;
}

void DungeonScene::RearrangeMinimapFocusCam()
{
	if (true == CheckRectRange(LogicHeader::FocusCamRange) && true == GameInput::Down(L"LeftMouseClick"))
	{
		float x = m_MinimapRender->Trans()->WPOS().x + 2.2f - m_MinimapRender->m_MinimapIndicatorPos.x;
		float y = m_MinimapRender->Trans()->WPOS().y + -2.2f - m_MinimapRender->m_MinimapIndicatorPos.y;
		m_MinimapRender->Trans()->WPOS({ x, y, 10.0f, 1.0f });
	}
}

void DungeonScene::MakeRandomMap(int _RoomCount /*= 1*/)
{
	m_MinimapRender->MakeRandomMap(_RoomCount);
}

void DungeonScene::MakeOrderMap(int _RoomCount, int* _WayArr, int _WayLength)
{
	m_MinimapRender->MakeOrderMap(_RoomCount, _WayArr, _WayLength);
}

void DungeonScene::CheckIndex()
{
	m_ScreenToXMouse = m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).x;
	m_ScreenToYMouse = m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).y;

	m_MinimapXIndex = (int)((m_ScreenToXMouse - m_MinimapRender->Trans()->WPOS().x) * 5);
	m_MinimapYIndex = (int)((m_ScreenToYMouse - m_MinimapRender->Trans()->WPOS().y) * 5);
}

void DungeonScene::RoomChooseCheck()
{
	if (true == GameInput::Down(L"LeftMouseClick"))
	{
		GameMiniMap::SKEY tempKey = GameMiniMap::SKEY(m_MinimapXIndex, m_MinimapYIndex);
		m_NextRoomIter = m_MinimapRender->m_AllRoom.find(tempKey.TileKey);

		bool Is_ConnectRoom = false;

		if (m_MinimapRender->m_AllRoom.end() == m_NextRoomIter)
		{
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			if (m_CurRoomIter->second->m_Room4Way[i] != nullptr && m_CurRoomIter->second->m_Room4Way[i] == m_NextRoomIter->second)
			{
				Is_ConnectRoom = true;
			}
		}

		if (!Is_ConnectRoom)
		{
			return;
		}

		if (true == m_CanMove && m_CurRoomIter->first != m_NextRoomIter->first)
		{
			ChangeState(ASTATE::MOVEROAD);
			m_GameMap->FadeCall(m_MainCam, 2.0f);
			m_CallRoadTimer = 2.0f;
			m_CallRoadTimerHalf = 1.0f;
			m_NextRoomIter->second->Move_On();
			m_CanMove = false;
		}
	}
}



bool DungeonScene::CheckRectRange(const GameRect& _Rect, bool _IsUI /*= true*/)
{
	if (_IsUI)
	{
		if (_Rect.LeftTop.x + m_MainCam->Trans()->WPOS().x > m_ScreenToXMouse || _Rect.RightBottom.x + m_MainCam->Trans()->WPOS().x < m_ScreenToXMouse)
		{
			return false;
		}

		if (_Rect.LeftTop.y + m_MainCam->Trans()->WPOS().y < m_ScreenToYMouse || _Rect.RightBottom.y + m_MainCam->Trans()->WPOS().y > m_ScreenToYMouse)
		{
			return false;
		}
	}
	else
	{
		if (_Rect.LeftTop.x > m_ScreenToXMouse || _Rect.RightBottom.x < m_ScreenToXMouse)
		{
			return false;
		}

		if (_Rect.LeftTop.y < m_ScreenToYMouse || _Rect.RightBottom.y > m_ScreenToYMouse)
		{
			return false;
		}
	}


	return true;
}

void DungeonScene::DebugRender()
{
	GameDebugEn::Draw2DFont(m_FPSstr, 20.0f, { 10.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	wchar_t buff[100];
	swprintf_s(buff, L"map x,y,z,w Pos : %.3f %.3f %.3f %.3f", m_GameMap->Trans()->WPOS().x, m_GameMap->Trans()->WPOS().y, m_GameMap->Trans()->WPOS().z, m_GameMap->Trans()->WPOS().w);
	GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(buff, L"MousePos3D %.3f, %.3f", GameWin::MainWindow()->MousePos3D().x, GameWin::MainWindow()->MousePos3D().y);
	GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 40.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(buff, L"CamPos %.3f, %.3f", m_MainCam->Trans()->WPOS().x, m_MainCam->Trans()->WPOS().y);
	GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(buff, L"CamScreenSize %.3f, %.3f"
		, m_MainCam->Size().x
		, m_MainCam->Size().y);
	GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 80.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(buff, L"ScreenToMousePos %.3f, %.3f"
		, m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).x
		, m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).y);
	GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 100.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (m_MinimapRender->m_AllRoomVec.size() != 0)
	{
		swprintf_s(buff, L"Room Pos X : %d, Pos Y : %d", (*(++m_MinimapRender->m_AllRoomVec.begin()))->m_Key.X, (*(++m_MinimapRender->m_AllRoomVec.begin()))->m_Key.Y);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 120.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"m_MinimapRender %.3f, %.3f %.3f, %.3f"
			, m_MinimapRender->Trans()->WPOS().x
			, m_MinimapRender->Trans()->WPOS().y
			, m_MinimapRender->Trans()->WPOS().z
			, m_MinimapRender->Trans()->WPOS().w);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 140.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"m_MinimapRender - mainCamPos X : %.3f, Y : %.3f"
			, m_MinimapRender->Trans()->WPOS().x - m_MainCam->Trans()->WPOS().x
			, m_MinimapRender->Trans()->WPOS().y - m_MainCam->Trans()->WPOS().y);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 160.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });


		swprintf_s(buff, L"Calc Index X : %d, Y : %d"
			, (int)((m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).x - m_MinimapRender->Trans()->WPOS().x - m_MainCam->Trans()->WPOS().x) * 5)
			, (int)((m_MainCam->ScreenPos3DToWorldPos(GameWin::MainWindow()->MousePos3D()).y - m_MinimapRender->Trans()->WPOS().y - m_MainCam->Trans()->WPOS().y) * 5));
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 180.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"Cur Dungeon State : %d, Cur Room X Index : %d, Cur Room Y Index : %d ", m_AState, m_CurRoomIter->second->m_Key.X, m_CurRoomIter->second->m_Key.Y);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 200.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"Player[0] Pos.x : %f, Player[3] Pos.x : %f", m_PlayerGroup[0]->Trans()->WPOS().x, m_PlayerGroup[0]->Trans()->WPOS().x);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 220.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		//위 연산에서 나온 값으로 칸당 0.2씩 나타내고 있음을 확인할 수 있다.
		swprintf_s(buff, L"indicator Pos.x : %f, indicator Pos.y : %f", m_MinimapRender->m_MinimapIndicatorPos.x, m_MinimapRender->m_MinimapIndicatorPos.y);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 240.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"CheckRectRange : %d", CheckRectRange(LogicHeader::FocusCamRange));
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 260.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"Indicator Index  X : %d, Y : %d", m_MinimapRender->m_MinimapIndicator->m_Key.X, m_MinimapRender->m_MinimapIndicator->m_Key.Y);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 280.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		swprintf_s(buff, L"SelectUIRender Pos  X : %.3f, Y : %.3f, Z : %.3f, W : %.3f", m_GameCombat->m_SelectRender->Trans()->WPOS().x, m_GameCombat->m_SelectRender->Trans()->WPOS().y, m_GameCombat->m_SelectRender->Trans()->WPOS().z, m_GameCombat->m_SelectRender->Trans()->WPOS().w);
		GameDebugEn::Draw2DFont(buff, 20.0f, { 10.0f, 300.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}


void DungeonScene::BattleState()
{
	// 몬스터가 모두 죽었다면
	if (m_CurCombat->m_MonsterData.size() == 0)
	{
		ChangeState(m_PrevAState);
		m_GameCombat->m_AllCombat.erase(m_GameCombat->m_CurCombat);
		int tempSize = (int)m_PlayerGroup.size();
		for (int i = 0; i < tempSize; i++)
		{
			m_PlayerGroup[i]->GetCom<HeroFsm>()->BattleEnd();
		}
	}

	if (nullptr == m_CurCombatActor)
	{
		if (m_CurCombat->m_SpeedStack.size() == 0)
		{
			m_GameCombat->TurnUpdate();
		}

		m_CurCombatActor = m_CurCombat->m_SpeedStack.top();
		m_CurCombat->m_SpeedStack.pop();
		m_GameCombat->ChangeSelectUI(m_CurCombatActor->Trans()->WPOS() - m_MainCam->Trans()->WPOS());
		m_BattleTimer = 2.0f;

		if (true == m_CurCombatActor->HasCom<HeroFsm>())
		{
			m_CurHeroFSM = m_CurCombatActor->GetCom<HeroFsm>();
			m_CurHeroCombat = m_CurCombatActor->GetCom<SHEROCOMBAT>();
			b_Is_Hero = true;
			b_Hero_Phase_End = false;
			m_GameCombat->m_HeroIndex = m_CurHeroCombat->m_CombatPosition;
			m_CurTargetRender = m_CurCombatActor->GetCom<TargetRender>();
		}
		else
		{
			b_Is_Hero = false;
			m_CurMonsterFSM = m_CurCombatActor->GetCom<MonsterFsm>();
			m_CurMonsterCombat = m_CurCombatActor->GetCom<SMONSTERCOMBAT>();
		}
	}

	// curspeed
	if (nullptr != m_CurCombatActor)
	{
		// 몇초기다렸다가
		// 몬스터는 인공지능으로 움직일테고
		// 플레이어는 조작을 받는걸 체크하는 로직들이 있어야 한다.
		if (false == b_Hero_Phase_End && true == b_Is_Hero)
		{
			m_BattleTimer = 2.0f;
		}

		if (m_BattleTimer < 0)
		{
			m_CurCombatActor = nullptr;
		}
	}


	int a = 0;

}

void DungeonScene::MoveRoomState()
{
	m_CallRoomTimer -= GameTime::DeltaTime();

	if (0.0f < m_CallRoomTimer && m_CallRoomTimer < m_CallRoomTimerHalf)
	{
		m_GameMap->Play_TutorialRoom(m_NextRoomIter->second->m_RoomSpriteName);
		m_NextRoomIter->second->Sprite(GameMiniMap::RoomSpriteName::room_curio);
		m_CallRoomTimer = -1.0f;
	}

	RoomChooseCheck();
}

void DungeonScene::CheckHeroCanMove()
{
	switch (m_AState)
	{
	case DungeonScene::ASTATE::BATTLE:
		for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
		{
			(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(true);
		}
		break;
	case DungeonScene::ASTATE::MOVEROOM:
		if (m_PlayerGroup[(int)m_PlayerGroup.size() - 1]->Trans()->WPOS().x < -5.5f || m_Is_Battle == true)
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(false);
			}
		}
		else
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->CanMove(false);
			}
		}

		if (m_PlayerGroup[0]->Trans()->WPOS().x > 5.5f || m_Is_Battle == true)
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(true);
			}
		}
		else
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->CanMove(true);
			}
		}
		break;
	case DungeonScene::ASTATE::MOVEROAD:
		if (m_PlayerGroup[(int)m_PlayerGroup.size() - 1]->Trans()->WPOS().x < -5.5f || m_Is_Battle == true)
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(false);
			}
		}
		else
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->CanMove(false);
			}
		}

		if (m_PlayerGroup[0]->Trans()->WPOS().x > 25.0f)
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(true);
			}
		}
		else
		{
			for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
			{
				(m_PlayerGroup[i]->GetCom<HeroFsm>())->CanMove(true);
			}
		}
		break;
	case DungeonScene::ASTATE::EVENT:
		for (int i = 0; i < (int)m_PlayerGroup.size(); i++)
		{
			(m_PlayerGroup[i]->GetCom<HeroFsm>())->StopMove(true);
		}
		break;
	default:
		break;
	}

}

void DungeonScene::MoveRoadState()
{
	m_MinimapRender->ChangeIndicatorPos(CheckMinimapIndicator(m_MainCam));
	m_CallRoadTimer -= GameTime::DeltaTime();

	if (0.0f < m_CallRoadTimer && m_CallRoadTimer < m_CallRoadTimerHalf)
	{
		m_GameMap->Play_Road_Render();
		m_CallRoadTimer = -1.0f;
	}

	if (true == GameInput::Press(L"MML"))
	{
		if (m_MainCamPos.x > 0)
		{
			m_MainCam->Trans()->LMOVETOTIMESPEED(CVector::LEFT, 2.0f);
			m_MidCam->Trans()->LMOVETOTIMESPEED(CVector::LEFT, 1.0f);
		}
	}

	if (true == GameInput::Press(L"MMR"))
	{
		if (m_MainCamPos.x < 20.0f)
		{
			m_MainCam->Trans()->LMOVETOTIMESPEED(CVector::RIGHT, 2.0f);
			m_MidCam->Trans()->LMOVETOTIMESPEED(CVector::RIGHT, 1.0f);
		}
	}

	if (true == GameInput::Down(L"Up") || true == GameInput::Down(L"UpKey"))
	{
		if (m_MainCamPos.x < 1.0f)
		{
			m_GameMap->OpenDoor(true);
			ChangeState(ASTATE::MOVEROOM);
			m_GameMap->FadeCall(m_MainCam, 2.0f);
			m_CallRoomTimer = 2.0f;
			m_CallRoomTimerHalf = 1.0f;
			m_NextRoomIter->second->Move_Off();
			m_CanMove = true;
		}

		if (19.0f < m_MainCamPos.x && m_MainCamPos.x < 21.0f)
		{
			m_GameMap->OpenDoor(false);
			ChangeState(ASTATE::MOVEROOM);
			m_GameMap->FadeCall(m_MainCam, 2.0f);
			m_CallRoomTimer = 2.0f;
			m_CallRoomTimerHalf = 1.0f;
			m_NextRoomIter->second->Move_Off();
			m_CurRoomIter = m_NextRoomIter;
			m_CanMove = true;
		}
	}
}


GameMiniMap::SKEY DungeonScene::CheckMinimapIndicator(SPtr<GameCam>& _Cam)
{
	int MoveRange = (int)(_Cam->Trans()->WPOS().x / 2.6f);

	GameMiniMap::ROADBLOCK* tempPtr = m_CurRoomIter->second->m_SamePosRoad;
	GameMiniMap::SKEY tempKey = tempPtr->m_Key;

	if (abs(m_CurRoomIter->second->m_Key.X) + abs(m_CurRoomIter->second->m_Key.Y)
		> abs(m_NextRoomIter->second->m_Key.X) + abs(m_NextRoomIter->second->m_Key.Y))
	{
		for (int i = 0; i < MoveRange; i++)
		{
			tempPtr = tempPtr->Prev;
			tempKey = tempPtr->m_Key;
		}
	}
	else
	{
		for (int i = 0; i < MoveRange; i++)
		{
			tempPtr = tempPtr->Next;
			tempKey = tempPtr->m_Key;
		}
	}

	return tempKey;
}

void DungeonScene::ChangeState(ASTATE _State)
{
	if (_State != m_AState)
	{
		m_PrevAState = m_AState;
	}

	switch (_State)
	{
	case DungeonScene::ASTATE::BATTLE:
		m_AState = ASTATE::BATTLE;
		m_Is_Battle = true;
		break;
	case DungeonScene::ASTATE::MOVEROOM:
		//게임 맵 클래스야 방을 만들어라.
		m_AState = ASTATE::MOVEROOM;
		m_Is_Battle = false;
		break;
	case DungeonScene::ASTATE::MOVEROAD:
		//게임 맵 클래스야 길을 만들어라.
		m_AState = ASTATE::MOVEROAD;
		m_Is_Battle = false;
		break;
	case DungeonScene::ASTATE::EVENT:
		break;
	default:
		break;
	}
}

void DungeonScene::HeroTargetInit(GameString& _HeroName, int _pos)
{
	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	if (!m_Dir.TryFindFileRecursive(_HeroName))
	{
		ASSERTMSG;
	}

	std::vector<GameFile> FSMlist = m_Dir.DirToAllDir();

	std::vector<GameFile> temp;

	GameString pngStr;
	GameString jsonStr;
	SPtr<GameActor> PlayerPTR;

	PlayerPTR = CreateActor(_HeroName);
	PlayerPTR->Trans()->LSCALE(LogicHeader::DefaultDarkImgSize);
	CVector tempVec = LogicHeader::PlayerStartPos;
	PlayerPTR->Trans()->WPOS({ tempVec.x - _pos * 1.1f,tempVec.y, tempVec.z - _pos * 0.01f, tempVec.w });

	SPtr<SHEROCOMBAT> HeroData = PlayerPTR->AddCom<SHEROCOMBAT>();
	ActorList::PlayerEnum HeroEnum = (ActorList::PlayerEnum)_pos;
	HeroData->m_HeroEnum = HeroEnum;

	SPtr<GameImage> RENDER = PlayerPTR->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PLAYER);
	SPtr<GameAni> ANICOM = PlayerPTR->AddCom<GameAni>();
	SPtr<GameJsonReader> Reader = PlayerPTR->AddCom<GameJsonReader>();
	SPtr<TextReader> txtReader = PlayerPTR->AddCom<TextReader>();
	SPtr<HeroFsm> fsm = PlayerPTR->AddCom<HeroFsm>(_HeroName);
	SPtr<TargetRender> targetRender = PlayerPTR->AddCom<TargetRender>();
	targetRender->HeroTargetInit(_HeroName);

	if (false == txtReader->LoadBinaryData(_HeroName, false))
	{
		txtReader->ParseHerotxt(m_Dir.CFullPath());
		txtReader->SaveBinaryData(_HeroName);
	}
	GameString fsmName;

	char find_json = 0;
	for (auto& FSM : FSMlist)
	{
		fsmName = _HeroName + L"_";
		m_Dir.Move(FSM.CFileName());
		temp = m_Dir.DirToAllFile();
		find_json = 0;

		for (auto& item : temp)
		{
			if (item.Ext() == L".png")
			{
				pngStr = item.CFileName() + L".png";
			}

			if (item.Ext() == L".json")
			{
				find_json = true;
				jsonStr = item.CFileName() + L".json";
			}
		}


		GameTexture::Load(m_Dir.CreateGameFilePath(pngStr));
		GameSprite::Create(m_Dir.CreateGameFilePath(pngStr), 1, 1);
		fsmName += FSM.CFileName();
		GameFile loadUvFile = m_Dir.CreateGameFileReadOnly(_HeroName + L"uvData.data");
		std::vector<CVector> tempVec;
		if (false != loadUvFile.Open())
		{
			int uvCount = 0;
			loadUvFile.Read(uvCount);
			tempVec.reserve(uvCount);

			CVector tempSVEC4;
			for (int i = 0; i < uvCount; i++)
			{
				loadUvFile.Read(tempSVEC4);
				tempVec.push_back(tempSVEC4);
			}
			RENDER->SpriteUv(pngStr, tempVec);
			ANICOM->CreateAni(fsmName, pngStr, 0, uvCount - 1, 0.03f, true);
			fsm->InputFSMName(fsmName);
		}
		else
		{
			int cutCount = 1;

			if (find_json)
			{
				Reader->ParseJson(m_Dir.CreateGameFilePath(jsonStr));
				tempVec = Reader->MakeUvData();
				RENDER->SpriteUv(pngStr, Reader->MakeUvData());
				cutCount = Reader->Count() - 1;

				GameFile saveUvFile = m_Dir.CreateGameFile(_HeroName + L"uvData.data");
				saveUvFile.Open();
				saveUvFile.Write((int)tempVec.size());

				for (int i = 0; i < (int)tempVec.size(); i++)
				{
					saveUvFile.Write(tempVec[i]);
				}

			}

			ANICOM->CreateAni(fsmName, pngStr, 0, cutCount, 0.03f, true);
			fsm->InputFSMName(fsmName);
		}

		m_Dir.MoveParent();
	}

	//hero armor수치, weapon수치에 따라 m_status 채워주기
	HeroData->LoadHeroStatus();
	//이름 넣어주기
	HeroData->HeroName(ActorList::PlayerName[_pos]);
	ANICOM->ChangeAni(_HeroName + L"_Idle");

	m_PlayerGroup.push_back(PlayerPTR);
}

void DungeonScene::DungeonInit(GameString& _DungeonName)
{
	SPtr<GameActor> PTR = CreateActor(_DungeonName + L"Map");
	PTR->Trans()->LPOS({ 0.0f, 0.0f, 0.0f });
	SPtr<GameMap> Map = PTR->AddCom<GameMap>();
	Map->MapLoad(_DungeonName);
	Map->DisplayMapAndUI();

	m_GameMap = Map;
	m_GameMapActor = PTR;
}

void DungeonScene::MonsterInit(GameString& _DungeounName, GameString& _MonsterName, int _DifficultLevel, int _Pos, int ScaleIndex /*= 0*/)
{
	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	if (!m_Dir.TryFindFileRecursive(L"Monster"))
	{
		ASSERTMSG;
	}
	if (!m_Dir.TryFindFileRecursive(_DungeounName))
	{
		ASSERTMSG;
	}
	m_Dir.Move(_MonsterName);

	SPtr<GameActor> EnemyPTR;
	EnemyPTR = CreateActor(_MonsterName);
	EnemyPTR->Trans()->LSCALE(LogicHeader::DefaultDarkMonsterImgSize[ScaleIndex]);

	CVector tempVec = LogicHeader::MonsterStartPos;
	EnemyPTR->Trans()->WPOS({ tempVec.x + _Pos * 1.1f,tempVec.y - LogicHeader::DefaultDarkMonsterYPos[ScaleIndex], tempVec.z - _Pos * 0.01f, tempVec.w });

	std::vector<GameFile> FSMlist = m_Dir.DirToAllDir();
	std::vector<GameFile> temp;

	GameString pngStr;
	GameString jsonStr;

	SPtr<GameImage> RENDER = EnemyPTR->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_MONSTER);
	SPtr<GameAni> ANICOM = EnemyPTR->AddCom<GameAni>();
	SPtr<GameJsonReader> Reader = EnemyPTR->AddCom<GameJsonReader>();
	SPtr<TextReader> txtReader = EnemyPTR->AddCom<TextReader>();
	SPtr<SMONSTERCOMBAT> MonsterCombat = EnemyPTR->AddCom<SMONSTERCOMBAT>();
	SPtr<MonsterFsm> fsm = EnemyPTR->AddCom<MonsterFsm>(_MonsterName);

	if (false == txtReader->LoadBinaryData(_MonsterName, true))
	{
		txtReader->ParseMonstertxt(m_Dir.CFullPath(), _DifficultLevel);
		txtReader->SaveBinaryData(_MonsterName);
	}

	for (auto& FSM : FSMlist)
	{
		m_Dir.Move(FSM.CFileName());
		temp = m_Dir.DirToAllFile();

		for (auto& item : temp)
		{
			if (item.Ext() == L".png")
			{
				pngStr = item.CFileName() + L".png";
			}

			if (item.Ext() == L".json")
			{
				jsonStr = item.CFileName() + L".json";
			}
		}

		GameTexture::Load(m_Dir.CreateGameFilePath(pngStr));
		GameSprite::Create(m_Dir.CreateGameFilePath(pngStr), 5, 3);

		GameFile loadUvFile = m_Dir.CreateGameFileReadOnly(_MonsterName + L"uvData.data");
		std::vector<CVector> tempVec;
		if (false != loadUvFile.Open())
		{
			int uvCount = 0;
			loadUvFile.Read(uvCount);
			tempVec.reserve(uvCount);

			CVector tempSVEC4;
			for (int i = 0; i < uvCount; i++)
			{
				loadUvFile.Read(tempSVEC4);
				tempVec.push_back(tempSVEC4);
			}
			RENDER->SpriteUv(pngStr, tempVec);
			ANICOM->CreateAni(FSM.CFileName(), pngStr, 0, uvCount - 1, 0.04f, true);
			fsm->InputFSMName(FSM.CFileName());
		}
		else
		{
			Reader->ParseJson(m_Dir.CreateGameFilePath(jsonStr));
			tempVec = Reader->MakeUvData();
			RENDER->SpriteUv(pngStr, Reader->MakeUvData());

			{
				GameFile saveUvFile = m_Dir.CreateGameFile(_MonsterName + L"uvData.data");
				saveUvFile.Open();
				saveUvFile.Write((int)tempVec.size());

				for (int i = 0; i < (int)tempVec.size(); i++)
				{
					saveUvFile.Write(tempVec[i]);
				}
			}
			ANICOM->CreateAni(FSM.CFileName(), pngStr, 0, (size_t)Reader->Count() - 1, 0.04f, true);
			fsm->InputFSMName(FSM.CFileName());
		}
		//Parse MonsterTex는 두번째 매개변수로 A,B,C,D를 구분함
		m_Dir.MoveParent();
		temp.clear();
	}

	MonsterCombat->MonsterName(ActorList::MonsterName[_Pos]);
	ANICOM->ChangeAni(FSMlist.begin()->CFileName());
	FSMlist.clear();
	//animation을 먼저 다 만들고
	//몬스터 정보를 불러오게 하자.

	m_MonsterGroup.push_back(EnemyPTR);
}
