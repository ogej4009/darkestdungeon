#pragma once
#include <GameComponent.h>
#include <LogicHeader.h>
#include <vector>

class MonsterData;
class HeroData;
class TextReader : public GameComponent
{
private:
	friend MonsterData;
	friend HeroData;
	friend SHEROCOMBAT;
	friend SMONSTERCOMBAT;

	GameString m_InfoName[4];
	GameString m_HerotxtName;
	MonsterData* m_ParseData;
	HeroData* m_ParseHeroData;

public:
	TextReader();
	~TextReader();

public:
	void ParseMonstertxt(const GameString& _FilePath, int DifficultyLevel);
	void ParseHerotxt(const GameString& _FilePath);
	void SaveBinaryData(GameString& _Name);
	bool LoadBinaryData(GameString& _Name, bool b_Is_Monster);

private:
	MonsterType CheckType(const GameString& _Type)
	{
		MonsterType tempType;

		if (_Type.m_Str.find(L"unholy") != std::wstring::npos)
		{
			tempType = MonsterType::unholy;
			return tempType;
		}
		
			tempType = MonsterType::human;
			return tempType;
	}

	SkillType CheckSkillType(const GameString& _Type)
	{
		SkillType tempType = SkillType::Max;

		if (_Type.m_Str.find(L"range") != std::wstring::npos)
		{
			tempType = SkillType::Range;
			return tempType;
		}

		if (_Type.m_Str.find(L"melee") != std::wstring::npos)
		{
			tempType = SkillType::Melee;
			return tempType;
		}

		ASSERTMSG;
		return tempType;
	}

	void readSkill(int _StartLine, GameString& _IndexStr, std::vector<size_t>* _enterCount);
};

