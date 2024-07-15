#pragma once
#include <GameMath.h>
#include <GameString.h>
#include <list>
#include <map>

enum class RENDERGROUP
{
	RG_PLAYER,
	RG_MONSTER,
	RG_TARGET,
	RG_PreMapUI,
	RG_MAP,
	RG_MAP2,
	RG_MidWall,
	RG_BackWall,
	RG_UI,
	RG_PostUI,
};

class LogicHeader
{
public:
	static const CVector ScreenSize;
	static const CVector UpScreenSize;
	static const CVector UpScreenPos;
	static const CVector CorridorSize;
	static const CVector CorridorMidSize;
	static const CVector DoorSize;
	static const CVector CorridorPos;
	static const CVector WindowSize;
	static const CVector PlayerStartPos;
	static const CVector PlayerCombatPos[4];
	static const CVector MonsterStartPos;
	static const CVector MonsterCombatPos[4];
	static const CVector MonsterTargetPos[4];
	static const CVector DefaultDarkImgSize;
	static const CVector DefaultDarkMonsterImgSize[2];
	static const float DefaultDarkMonsterYPos[2];
	static const CVector SelectUIPlayerXOffset;
	static const CVector SelectUIMonsterXOffset;
	static const GameRect MinimapRange;
	static const GameRect FocusCamRange;
	static const GameRect TargetRange[4];
	static const float MinimapZpos;
	static const float PlayerZpos;
	static const float MonsterZpos;
	static const float BackBottomZpos;

	
};


enum class MonsterType
{
	human,
	unholy,
	Max
};

enum class SkillType
{
	Melee,
	Range,
	Max,
};

class CombatStat
{
public:
	float m_Hp;
	float m_Def;
	float m_Prot;
	float m_Speed;
	float m_StunRes;
	float m_PoisonRes;
	float m_BleedRes;
	float m_DebuffRes;
	float m_MoveRes;
};


class CombatSkill
{
public:
	GameString m_Name;
	float m_ATK;
	SkillType m_SkillType;
	int m_DmgMin;
	int m_DmgMax;
	float m_Crit;
	std::list<GameString> m_Effect;
	bool m_bIsMoveable;
	bool m_CanHeal;
	int m_MoveEffect[2];
	int m_HealMin;
	int m_HealMax;
	bool m_LaunchPos[4];
	bool m_TargetPos[4];
	bool m_AOE;

public:
	CombatSkill() : m_bIsMoveable(false), m_AOE(false), m_CanHeal(false)
	{
		for (size_t i = 0; i < 4; i++)
		{
			m_TargetPos[i] = false;
			m_LaunchPos[i] = false;
		}
	}
};

class SGAMECOMBAT;
class SMONSTERCOMBAT;
class TextReader;
class MonsterData
{
private:
	friend TextReader;
	friend SMONSTERCOMBAT;
	friend SGAMECOMBAT;

public:
	unsigned int m_DisplaySize;
	MonsterType m_MonsterType;
	CombatStat m_Status;
	std::vector<CombatSkill> m_Skill;
	GameString m_MonsterName;

public:
	MonsterData()
	{
		m_Skill.reserve(4);
	}

	MonsterData(const MonsterData& _Other)
	{
		m_DisplaySize = _Other.m_DisplaySize;
		m_MonsterType = _Other.m_MonsterType;
		m_Status = _Other.m_Status;
		m_Skill = _Other.m_Skill;
	}

	~MonsterData()
	{
		if (0 != m_Skill.size())
		{
			m_Skill.clear();
		}

	}
};



class TextReader;
class SGAMECOMBAT;
class SHEROCOMBAT;
class HeroData
{
private:
	friend TextReader;
	friend SGAMECOMBAT;
	friend SHEROCOMBAT;

private:
	class HeroStat : public CombatStat
	{
	public:
		float m_DeathBlow;
		float m_DiseaseRes;
		float m_TrapSkill;
		float m_Crit;
		float m_MinDmg;
		float m_MaxDmg;
		float m_Stress;
		float m_BonusHit;
	};

	class HeroWeapon
	{
	public:
		GameString m_Name;
		float m_Atk;
		float m_MinDmg;
		float m_MaxDmg;
		float m_Crit;
		float m_Speed;
		int m_WeaponUpgradeCode;
	};

	class HeroArmor
	{
	public:
		GameString m_Name;
		float m_Def;
		float m_Prot;
		float m_Hp;
		float m_Speed;
		int m_ArmorUpgradeCode;
	};

	class HeroSkill : public CombatSkill
	{
	public:
		bool m_Generation_guarantee;
		bool m_Is_Stall_Invalidating;
		bool m_IsBenefitSkill;
		bool m_Is_Crit_Valid;
		int m_Per_Battle_Limit;
		int m_SkillLevel;
		float m_DmgMod;

		HeroSkill()
			: m_Per_Battle_Limit(-1),
			m_SkillLevel(-1),
			m_Is_Stall_Invalidating(true),
			m_Is_Crit_Valid(false),
			m_IsBenefitSkill(false)
		{

		}
	};

private:
	//한 스킬당 5개가 존재한다. 이름으로 먼저 나누고 각자 5개씩 들고 있으면 된다.
	std::map<GameString, std::vector<HeroSkill*>> m_Skill;
	std::map<GameString, std::vector<HeroSkill*>>::iterator m_SkillStart;
	std::map<GameString, std::vector<HeroSkill*>>::iterator m_SkillEnd;
	std::vector<HeroArmor*> m_Armor;
	std::vector<HeroWeapon*> m_Weapon;
	GameString m_Effect;

	//영웅 스테이터스는 다른 정보 다 읽고 마지막에 세팅하자.
	HeroStat m_Status;
	GameString m_Name;

public:
	HeroData()
	{
		m_Armor.reserve(5);
		m_Weapon.reserve(5);
	}
	~HeroData()
	{
		m_SkillStart = m_Skill.begin();
		m_SkillEnd = m_Skill.end();

		for (; m_SkillStart != m_SkillEnd; ++m_SkillStart)
		{
			for (auto& skill : m_SkillStart->second)
			{
				delete skill;
				skill = nullptr;
			}
		}

		m_Skill.clear();

		for (auto& armor : m_Armor)
		{
			delete armor;
			armor = nullptr;
		}

		m_Armor.clear();

		for (auto& weapon : m_Weapon)
		{
			delete weapon;
			weapon = nullptr;
		}

		m_Weapon.clear();
	}

};

class ActorList
{
public:
	enum class MapEnum
	{
		cove,
		crypts,
		darkestdungeon,
		warrens,
		weald,
		common,
		Max
	};

	enum class PlayerEnum
	{
		crusader,
		highwayMan,
		vestal,
		plague_doctor,
		Max
	};

	enum class CryptMonser
	{
		skeleton_arbalist,
		skeleton_bearer,
		skeleton_captain,
		skeleton_common,
		skeleton_courtier,
		skeleton_defender,
		skeleton_militia,
		skeleton_spear,
		Max
	};

public:
	static GameString PlayerList[4];
	static GameString PlayerListKor[4];
	static GameString CryptMonsterList[8];
	static GameString mapList[6];
	static GameString PlayerName[4];
	static GameString MonsterName[4];
	static GameString TargetSkillName[7];

public:
	static const GameString crusader_SkillName[7];
	static const GameString highwayman_SkillName[7];
	static const GameString vestal_SkillName[7];
	static const GameString plague_doctor_SkillName[7];

};

enum class HeroFSMLIST
{
	Afflicted,
	Camp,
	Combat,
	Death,
	Defend,
	Heroic,
	Idle,
	Investigate,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5,
	Skill6,
	Skill7,
	Walk,
	MAX,
};


enum class MonsterFSMLIST
{
	Combat,
	Death,
	Defend,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	MAX,
};