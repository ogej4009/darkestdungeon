#pragma once
#include <vector>
#include <list>
#include <stack>
#include <LogicHeader.h>
#include <GameLogic.h>
#include <GameMiniMap.h>

class GameCam;
class GameSprite;
class SHEROCOMBAT;
class SMONSTERCOMBAT;
class GameImage;
class GameFont;
class DungeonScene;
class SGAMECOMBAT : public GameLogic
{
private:
	friend SMONSTERCOMBAT;
	friend SHEROCOMBAT;
	friend DungeonScene;
	//전투,캐릭터 관련 요소들을 컨트롤 함, DungeonCom에서 해당 클래스를 생성해서 동작시킴

private:
	//전투 처리 관련
	int m_HeroCount;
	int m_MonsterCount;
	std::vector<SPtr<SHEROCOMBAT>> m_HeroData;
	std::vector<SPtr<SMONSTERCOMBAT>> m_MonsterData;
	std::vector<SPtr<SMONSTERCOMBAT>>::iterator m_MonsterDataIterator;
	std::list<std::pair<float, GameActor*>> m_SpeedArrange;
	std::vector<GameActor*> m_HeroActor;
	std::vector<GameActor*> m_MonsterActor;
	std::vector<GameActor*>::iterator m_MonsterActorIter;
	std::stack<GameActor*> m_SpeedStack;
	SPtr<GameCam> m_MainCam;

	//캐릭터 선택 UI관련
	SPtr<GameSprite> m_SelectSprite[4];
	SPtr<GameImage> m_SelectRender;
	SPtr<GameActor> m_CombatActor;

	float m_SelectUITimer;
	float m_MaxSelectUIWidth;
	float m_MinSelectUIWidth;
	CVector m_SelectUISize;
	
	std::map<__int64, SPtr<SGAMECOMBAT>> m_AllCombat;
	std::map<__int64, SPtr<SGAMECOMBAT>>::iterator m_CurCombat;

	//영웅 UI관련
	class SHEROUISPRITE
	{
	private:
		friend SGAMECOMBAT;

	private:
		SPtr<GameSprite> m_HeroArmor[5];
		SPtr<GameSprite> m_HeroWeapon[5];
		SPtr<GameSprite> m_HeroAbility[7];
		SPtr<GameSprite> m_HeroAbilityBlack[7];
		SPtr<GameSprite> m_HeroPortrait;
	};
	
	static const GameString m_ArmorSpriteName[5];
	static const GameString m_WepaonSpriteName[5];
	static const GameString m_SkillName[7];
	static const GameString m_BlackSkillName[7];
	SHEROUISPRITE m_HeroUISprite[4];
	SPtr<GameSprite> m_MoveSprite;
	SPtr<GameSprite> m_AbilityPassSprite;
	SPtr<GameSprite> m_SelectAbilityprite;


	SPtr<GameImage> m_HeroArmorRender;
	SPtr<GameImage> m_HeroWeaponRender;
	SPtr<GameImage> m_HeroAbilityRender[4];
	SPtr<GameImage> m_HeroPortraitRender;
	SPtr<GameImage> m_MoveSpritRender;
	SPtr<GameImage> m_AbilityPassSpriteRender;
	SPtr<GameImage> m_SelectAbilitypriteRender;

	const static CVector m_AbilityPos[4];

	SPtr<SHEROCOMBAT> m_CurHeroCombat;
	SPtr<SMONSTERCOMBAT> m_CurMonsterCombat;
	HeroData::HeroSkill* m_CurSkill;
	CombatSkill* m_CurMonsterSkill;
	int m_MaxHp;
	int m_HeroIndex;
	int m_SelectSkillIndex;
	bool b_Is_Battle;
	bool b_Can_Activate_Skill[4];
	bool b_Is_Hero_SKill_Target[4];
	bool b_Is_Monster_SKill_Target[4];
	std::vector<int> m_MonsterTarget;

	//Target관련
	SPtr<GameSprite> m_TargetSprite[4];
	SPtr<GameImage> m_TargetRender[4];

public:
	SGAMECOMBAT();
	~SGAMECOMBAT();

public:
	void Init() override;
	void Update() override;

public:
	void TurnUpdate();
	void CreateCombat(const GameMiniMap::SKEY& _Key,std::vector<SPtr<GameActor>>& _Actor);
	bool CheckCombat(const GameMiniMap::SKEY& _Key);
	bool CheckCombat(const int _X, const int _Y)
	{
		GameMiniMap::SKEY tempKey = GameMiniMap::SKEY(_X, _Y);
		return CheckCombat(tempKey);
	}
	void StartBattle(SPtr<GameCam> _Cam);
	void ChangeSelectUI(const CVector& _Pos, int _SpriteIndex = 0);
	void CheckSkillTarget(std::vector<bool>* _TargetPos);
	std::vector<int> CheckMonsterSkillTarget();
	bool CheckSkillLaunch(GameActor& m_CurHeroActor, int _SkillNum, int _SkillLevel = 0, bool _ChangeCurSkill = true);
	void CalcDamge(int _TargetPos);
	void CalcHeroDamge(CombatSkill* _MonsterSKill, int _TargetPos);
	void CloseTarget();
	void RearragneMonster();

private:
	void PrintFont(int _HeroIndex = 0);
	void PrintHeroUI(int _HeroIndex = 0);
	void PrintTarget();
};

class DungeonScene;
class SHEROCOMBAT : public GameLogic
{
private:
	friend TextReader;
	friend SGAMECOMBAT;
	friend DungeonScene;

	//자료는 textreader에서 new로 heap에 생성해놓음, 여기선 포인터만 가져다 쓴다.
	HeroData* m_HeroData;
	int m_HeroArmorIndex;
	int m_HeroWeaponIndex;
	int m_CombatPosition;
	ActorList::PlayerEnum m_HeroEnum;

public:
	SHEROCOMBAT() : m_HeroArmorIndex(0), m_HeroWeaponIndex(0)
	{

	}

	~SHEROCOMBAT()
	{

	}

public:
	void LoadHeroStatus();
	void HeroName(const GameString& _Name)
	{
		m_HeroData->m_Name = _Name;
	}
};

class SMONSTERCOMBAT : public GameLogic
{
private:
	friend TextReader;
	friend SGAMECOMBAT;
	//자료는 textreader에서 new로 heap에 생성해놓음, 해제는 textreader가 알아서 하고
	//몬스터 데이터는 동일 데이터를 덮어쓸 일이 생길 수 있으므로 값형으로 바꿔놓았음
	MonsterData m_MonsterData;
	int m_CombatPosition;

public:
	SMONSTERCOMBAT()
	{
	}

	SMONSTERCOMBAT(const SMONSTERCOMBAT& _Other)
	{
		m_MonsterData = _Other.m_MonsterData;
	}

	~SMONSTERCOMBAT()
	{
	}

	void MonsterName(const GameString& _Name)
	{
		m_MonsterData.m_MonsterName = _Name;
	}

	int SkillCount()
	{
		return (int)m_MonsterData.m_Skill.size();
	}
};

