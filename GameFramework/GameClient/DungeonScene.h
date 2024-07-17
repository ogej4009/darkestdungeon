#pragma once
#include <GameScene.h>
#include <GameMiniMap.h>
#include <LogicHeader.h>
#include <map>

class TargetRender;
class MonsterFsm;
class GameCam;
class GameMap;
class GameActor;
class SHEROCOMBAT;
class SMONSTERCOMBAT;
class GameActor;
class STITLECOM;
class DungeonScene;
class GameSprite;
class GameImage;
class GameActor;
class SGAMECOMBAT;
class HeroFsm;
class DungeonScene : public GameScene::SceneComponent
{
private:
	friend GameMiniMap;
	friend SGAMECOMBAT;
	friend SHEROCOMBAT;

private:
	enum class ASTATE
	{
		BATTLE,
		//방 안에서 움직일때
		MOVEROOM,
		//길에서 움직일때
		MOVEROAD,
		EVENT,
	};

	enum class BATTLESTATE
	{
		ACTORSELECT,
		SELECTICONMOVE,
		TURNUPDATE,
		ACTION,
	};

private:
	//방의 위치 키와 해당 방에 있어야할 정보를 들고 있는 멤버
	//던전은 여러개의 컴뱃정보를 생성시에 가지고 있으며 방의 조우가 확인되면 해당 컴뱃 정보를 불러와서 전투를 진행한다.
	std::map< unsigned __int64, SGAMECOMBAT*> m_AllCombat;
	SGAMECOMBAT* m_CurCombat;	
	GameActor* m_CurCombatActor;
	SPtr<GameActor> m_CurUnitPointer;

	float m_FPSTimer;
	float m_FPS;
	GameString m_FPSstr;

	//Init 관련 멤버들
private:
	std::vector<SPtr<GameActor>> m_PlayerGroup;
	std::vector<SPtr<GameActor>> m_MonsterGroup;

	SPtr<GameActor> m_GameMapActor;
	SPtr<GameMap> m_GameMap;
	SPtr<SGAMECOMBAT> m_GameCombat;
	SPtr<GameCam> m_MainCam;
	SPtr<GameCam> m_MidCam;
	SPtr<GameCam> m_BackCam;
	SPtr<GameCam> m_UICam;

	CVector m_MainCamPos;
	CVector m_PrevMousePos;

	GameMiniMap* m_MinimapRender;
	GameActor* m_LoadingImage;

	int m_MinimapXIndex;
	int m_MinimapYIndex;
	float m_ScreenToXMouse;
	float m_ScreenToYMouse;

	//룸 이동 관련 친구들
	bool m_CanMove;
	bool m_Is_Battle;

	float m_CallRoadTimer;
	float m_CallRoadTimerHalf;
	float m_CallRoomTimer;
	float m_CallRoomTimerHalf;
	
	std::map<__int64, GameMiniMap::ROOM*>::iterator m_NextRoomIter;
	std::map<__int64, GameMiniMap::ROOM*>::iterator m_CurRoomIter;


	//battle 관련
	float m_BattleTimer;
	bool b_Is_Hero;
	SPtr<HeroFsm> m_CurHeroFSM;
	SPtr<MonsterFsm> m_CurMonsterFSM;
	SPtr<SHEROCOMBAT> m_CurHeroCombat;
	SPtr<SMONSTERCOMBAT> m_CurMonsterCombat;
	SPtr<TargetRender> m_CurTargetRender;

	bool b_Can_Activate_Skill;
	HeroFSMLIST m_DUNGEON_NextFSM;
	bool b_Hero_Phase_End;
	CombatSkill* m_CurMonsterSKill;
	std::vector<bool> m_TargetPos;
	int m_CurSkillCount;

public:
	ASTATE m_AState;
	ASTATE m_PrevAState;

public:
	DungeonScene();
	~DungeonScene();

public:
	void Loading() override;
	void Update() override;
	void DebugRender() override;

private:
	//state 관련
	void BattleState();
	void MoveRoomState();
	void MoveRoadState();
	void ChangeState(ASTATE _State);

	GameMiniMap::SKEY CheckMinimapIndicator(SPtr<GameCam>& _Cam);

	//Init관련
	void HeroTargetInit(GameString& _HeroName, int _pos);
	void DungeonInit(GameString& _DungeonName);
	void MonsterInit(GameString& _DungeounName, GameString& _MonsterName, int _DifficultLevel, int _Pos, int ScaleIndex = 0);
	void MinimapInit();
	void CombatInit();
	void MakeRandomMap(int _RoomCount = 1);
	void MakeOrderMap(int _RoomCount, int* _WayArr, int _WayLength);

	//전투관련
	void CheckCombat();
	void CheckHeroSkill();
	void CheckMonsterSkill();
	void CheckHeroIndex();

	//잡다한 필요기능
	void RearrageCamAndFade();
	bool CheckRectRange(const GameRect& _Rect, bool _IsUI = true);
	void CheckIndex();
	void RearrangeMinimapFocusCam();
	void RoomChooseCheck();
	void MoveMinimap();
	void CheckHeroCanMove();

};

