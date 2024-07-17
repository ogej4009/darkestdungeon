#include <GameFile.h>
#include <GameRef.h>
#include <GameActor.h>
#include <GameDic.h>

#include "TextReader.h"
#include "Combat.h"

#define WIDE_ENDLINE L"\r\n"

TextReader::TextReader()
{
	m_InfoName[0] = L"_A.info.darkest";
	m_InfoName[1] = L"_B.info.darkest";
	m_InfoName[2] = L"_C.info.darkest";
	m_InfoName[3] = L"_D.info.darkest";
	m_HerotxtName = L".info.darkest";
}

TextReader::~TextReader()
{
	if (nullptr != m_ParseData)
	{
		delete m_ParseData;
		m_ParseData = nullptr;
	}

	if (nullptr != m_ParseHeroData)
	{
		delete m_ParseHeroData;
		m_ParseHeroData = nullptr;
	}
}

void TextReader::ParseMonstertxt(const GameString& _FilePath, int DifficultyLevel)
{
	GameString TargetPath = _FilePath;
	TargetPath += L"\\";

	switch (DifficultyLevel)
	{
	case 0:
		m_InfoName[0] = _FilePath.PathData().ArrFile + m_InfoName[0].m_Str;
		TargetPath += m_InfoName[0];
		break;
	case 1:
		m_InfoName[1] = _FilePath.PathData().ArrFile + m_InfoName[1].m_Str;
		TargetPath += m_InfoName[1];
		break;
	case 2:
		m_InfoName[2] = _FilePath.PathData().ArrFile + m_InfoName[2].m_Str;
		TargetPath += m_InfoName[2];
		break;
	case 3:
		m_InfoName[3] = _FilePath.PathData().ArrFile + m_InfoName[3].m_Str;
		TargetPath += m_InfoName[3];
		break;

	default:
		break;
	}

	GameFile txtFile = GameFile(TargetPath, L"rb");
	GameString innerString = txtFile.GetFileToText();
	m_ParseData = new MonsterData();

	size_t sizeIndex;
	size_t end;
	GameString indexStr;

	std::vector<size_t> enterCount = innerString.FindToCount(WIDE_ENDLINE);

	{
		sizeIndex = innerString.Find(L".size");
		end = sizeIndex + 7;
		indexStr = innerString.GetCutText(sizeIndex + 6, sizeIndex + 6);
		m_ParseData->m_DisplaySize = std::stoi(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".id");
		end = enterCount[1];
		indexStr = innerString.GetCutText(sizeIndex + 5, end - 2);
		m_ParseData->m_MonsterType = CheckType(indexStr);
	}

	CombatStat tempStat = CombatStat();

	{
		sizeIndex = innerString.Find(L".hp");
		end = innerString.Find(L".def");
		indexStr = innerString.GetCutText(sizeIndex + 4, end - 2);
		tempStat.m_Hp = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".def");
		end = innerString.Find(L".prot");
		indexStr = innerString.GetCutText(sizeIndex + 4, end - 3);
		tempStat.m_Def = std::stof(indexStr.m_Str.c_str()) / 100.0f;
	}

	{
		sizeIndex = innerString.Find(L".prot");
		end = innerString.Find(L".spd");
		indexStr = innerString.GetCutText(sizeIndex + 6, end - 2);
		tempStat.m_Prot = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".spd");
		end = innerString.Find(L".stun_resist");
		indexStr = innerString.GetCutText(sizeIndex + 5, end - 2);
		tempStat.m_Speed = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".stun_resist");
		end = innerString.Find(L".poison_resist");
		indexStr = innerString.GetCutText(sizeIndex + 13, end - 3);
		tempStat.m_StunRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".poison_resist");
		end = innerString.Find(L".bleed_resist");
		indexStr = innerString.GetCutText(sizeIndex + 15, end - 3);
		tempStat.m_PoisonRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".bleed_resist");
		end = innerString.Find(L".debuff_resist");
		indexStr = innerString.GetCutText(sizeIndex + 14, end - 3);
		tempStat.m_BleedRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".debuff_resist");
		end = innerString.Find(L".move_resist");
		indexStr = innerString.GetCutText(sizeIndex + 15, end - 3);
		tempStat.m_DebuffRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".move_resist");
		end = enterCount[2];
		indexStr = innerString.GetCutText(sizeIndex + 13, end - 2);
		tempStat.m_MoveRes = std::stof(indexStr.m_Str.c_str());
	}

	m_ParseData->m_Status = tempStat;

	{
		std::vector<size_t> skillIndex = innerString.FindToCount(L"skill: .id");

		GameString subIndexStr;
		for (size_t i = 0; i < skillIndex.size(); i++)
		{
			sizeIndex = enterCount[2 + i];
			end = enterCount[3 + i];
			indexStr = innerString.GetCutText(sizeIndex + 14, end + 1);


			CombatSkill* tempData = new CombatSkill();

			{
				sizeIndex = indexStr.Find(L".id");
				end = indexStr.Find(L".type");
				tempData->m_Name = indexStr.GetCutText(sizeIndex + 1, end - 3);
			}

			{
				sizeIndex = indexStr.Find(L".type");
				end = indexStr.Find(L".atk");
				tempData->m_SkillType = CheckSkillType(indexStr.GetCutText(sizeIndex + 7, end - 3));
			}

			{
				sizeIndex = indexStr.Find(L".atk");
				end = indexStr.Find(L".dmg");
				subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
				tempData->m_ATK = std::stof(subIndexStr.m_Str.c_str());
			}

			{
				sizeIndex = indexStr.Find(L".dmg");
				end = indexStr.Find(L".crit");
				subIndexStr = indexStr.GetCutText(sizeIndex + 5, sizeIndex + 6);
				tempData->m_DmgMin = std::stoi(subIndexStr.m_Str.c_str());

				subIndexStr = indexStr.GetCutText(sizeIndex + 7, sizeIndex + 8);
				tempData->m_DmgMax = std::stoi(subIndexStr.m_Str.c_str());
			}

			{
				sizeIndex = indexStr.Find(L".crit");
				end = indexStr.Find(L".effect");
				if (end != std::wstring::npos)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
					tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());

					sizeIndex = indexStr.Find(L".effect");
					end = indexStr.Find(L".launch");
					subIndexStr = indexStr.GetCutText(sizeIndex + 9, end + 7);

					GameString twoSubIndexStr;

					std::vector<size_t> skillCount = subIndexStr.FindToCount(L"\" \"");
					if (skillCount.size() != 0)
					{
						size_t count = skillCount.size();

						end = skillCount[0];
						twoSubIndexStr = subIndexStr.GetCutText(0, end - 1);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());

						for (size_t i = 0; i < count - 1; i++)
						{
							sizeIndex = skillCount[0 + i];
							end = skillCount[1 + i];
							twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 1);
							tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
						}

						sizeIndex = skillCount[count - 1];
						end = subIndexStr.Find(L".launch");
						twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 3);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
					}
					else
					{
						tempData->m_Effect.push_back(subIndexStr.m_Str.c_str());
					}

				}
				else
				{
					end = indexStr.Find(L".launch");
					subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 4);
					tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
				}

			}

			{
				sizeIndex = indexStr.Find(L".launch");
				end = indexStr.Find(L".target");
				size_t size = end - sizeIndex;
				size -= 9;

				for (size_t i = 0; i < size; i++)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
					tempData->m_LaunchPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
				}

			}

			{
				sizeIndex = indexStr.Find(L".target");
				end = indexStr.Find(L".move");

				if (end != std::wstring::npos)
				{
					tempData->m_bIsMoveable = true;
					size_t size = end - sizeIndex;
					size -= 9;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						if (subIndexStr.m_Str[0] == L'~')
						{
							tempData->m_AOE = true;
							continue;
						}
						tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
					}

					sizeIndex = indexStr.Find(L".move");
					end = indexStr.Find(L"\r\n");
					subIndexStr = indexStr.GetCutText(sizeIndex + 6, sizeIndex + 6);
					tempData->m_MoveEffect[0] = std::stoi(subIndexStr.m_Str.c_str());
					subIndexStr = indexStr.GetCutText(sizeIndex + 8, sizeIndex + 8);
					tempData->m_MoveEffect[1] = std::stoi(subIndexStr.m_Str.c_str());
				}
				else if (indexStr.Find(L".is_crit_valid ") != std::wstring::npos)
				{
					end = indexStr.Find(L".is_crit_valid");
					size_t size = end - sizeIndex;
					size -= 9;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						if (subIndexStr.m_Str[0] == L'~')
						{
							tempData->m_AOE = true;
							continue;
						}
						tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
					}
				}
				else
				{
					end = indexStr.Find(L"\r\n");
					size_t size = end - sizeIndex;
					size -= 8;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						if (subIndexStr.m_Str == L"~")
						{
							tempData->m_AOE = true;
							continue;
						}
						tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;

					}
				}

			}
			m_ParseData->m_Skill.push_back(*tempData);
			delete tempData;
		}
		Actor()->GetCom<SMONSTERCOMBAT>()->m_MonsterData = *m_ParseData;
	}
}

void TextReader::ParseHerotxt(const GameString& _FilePath)
{
	GameString TargetPath = _FilePath;
	TargetPath += L"\\";
	TargetPath += _FilePath.PathData().ArrFile + m_HerotxtName.m_Str;

	GameFile txtFile = GameFile(TargetPath, L"rb");
	GameString innerString = txtFile.GetFileToText();
	m_ParseHeroData = new HeroData();

	size_t sizeIndex;
	size_t end;
	GameString indexStr;
	GameString subIndexStr;

	std::vector<size_t> enterCount = innerString.FindToCount(WIDE_ENDLINE);

	{
		sizeIndex = innerString.Find(L".stun");
		end = innerString.Find(L".poison");
		indexStr = innerString.GetCutText(sizeIndex + 6, end - 3);
		m_ParseHeroData->m_Status.m_StunRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".poison");
		end = innerString.Find(L".bleed");
		indexStr = innerString.GetCutText(sizeIndex + 8, end - 3);
		m_ParseHeroData->m_Status.m_PoisonRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".bleed");
		end = innerString.Find(L".disease");
		indexStr = innerString.GetCutText(sizeIndex + 7, end - 3);
		m_ParseHeroData->m_Status.m_BleedRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".disease");
		end = innerString.Find(L".move");
		indexStr = innerString.GetCutText(sizeIndex + 9, end - 3);
		m_ParseHeroData->m_Status.m_DiseaseRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".move");
		end = innerString.Find(L".debuff");
		indexStr = innerString.GetCutText(sizeIndex + 6, end - 3);
		m_ParseHeroData->m_Status.m_MoveRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".debuff");
		end = innerString.Find(L".death_blow");
		indexStr = innerString.GetCutText(sizeIndex + 8, end - 3);
		m_ParseHeroData->m_Status.m_DebuffRes = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".death_blow");
		end = innerString.Find(L".trap");
		indexStr = innerString.GetCutText(sizeIndex + 12, end - 3);
		m_ParseHeroData->m_Status.m_DeathBlow = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".trap");
		end = enterCount[0];
		indexStr = innerString.GetCutText(sizeIndex + 6, end - 2);
		m_ParseHeroData->m_Status.m_TrapSkill = std::stof(indexStr.m_Str.c_str());
	}

	{
		sizeIndex = innerString.Find(L".effects");
		end = enterCount[1];
		indexStr = innerString.GetCutText(sizeIndex + 10, end - 2);
		m_ParseHeroData->m_Effect = indexStr.m_Str.c_str();
	}

	//weapon
	{
		sizeIndex = enterCount[1];
		end = enterCount[2];
		indexStr = innerString.GetCutText(sizeIndex + 2, end - 1);
		HeroData::HeroWeapon* tempData = new HeroData::HeroWeapon();

		{
			sizeIndex = indexStr.Find(L".name");
			end = indexStr.Find(L".atk");
			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
			tempData->m_Name = subIndexStr;
		}

		{
			sizeIndex = indexStr.Find(L".atk");
			end = indexStr.Find(L".dmg");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
			tempData->m_Atk = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".dmg");
			end = indexStr.Find(L".crit");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 4);
			tempData->m_MinDmg = std::stof(subIndexStr.m_Str.c_str());

			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 2);
			tempData->m_MaxDmg = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".crit");
			end = indexStr.Find(L".spd");
			subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
			tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".spd");
			end = enterCount[2];
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 1);
			tempData->m_Speed = std::stof(subIndexStr.m_Str.c_str());
		}

		tempData->m_WeaponUpgradeCode = -1;
		m_ParseHeroData->m_Weapon.push_back(tempData);
	}

	for (int i = 0; i < 4; i++)
	{
		sizeIndex = enterCount[2 + i];
		end = enterCount[3 + i];
		indexStr = innerString.GetCutText(sizeIndex + 2, end - 1);
		HeroData::HeroWeapon* tempData = new HeroData::HeroWeapon();

		{
			sizeIndex = indexStr.Find(L".name");
			end = indexStr.Find(L".atk");
			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
			tempData->m_Name = subIndexStr;
		}

		{
			sizeIndex = indexStr.Find(L".atk");
			end = indexStr.Find(L".dmg");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
			tempData->m_Atk = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".dmg");
			end = indexStr.Find(L".crit");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 4);
			tempData->m_MinDmg = std::stof(subIndexStr.m_Str.c_str());

			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 2);
			tempData->m_MaxDmg = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".crit");
			end = indexStr.Find(L".spd");
			subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
			tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".spd");
			end = indexStr.Find(L".upgradeRequirementCode");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 2);
			tempData->m_Speed = std::stof(subIndexStr.m_Str.c_str());
		}

		tempData->m_WeaponUpgradeCode = i;
		m_ParseHeroData->m_Weapon.push_back(tempData);
	}

	//armor
	{
		sizeIndex = enterCount[6];
		end = enterCount[7];
		indexStr = innerString.GetCutText(sizeIndex + 2, end - 1);
		HeroData::HeroArmor* tempData = new HeroData::HeroArmor();

		{
			sizeIndex = indexStr.Find(L".name");
			end = indexStr.Find(L".def");
			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
			tempData->m_Name = subIndexStr;
		}

		{
			sizeIndex = indexStr.Find(L".def");
			end = indexStr.Find(L".prot");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
			tempData->m_Def = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".prot");
			end = indexStr.Find(L".hp");
			subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 1);
			tempData->m_Prot = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".hp");
			end = indexStr.Find(L".spd");
			subIndexStr = indexStr.GetCutText(sizeIndex + 4, end - 2);
			tempData->m_Hp = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".spd");
			end = enterCount[7];
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 1);
			tempData->m_Speed = std::stof(subIndexStr.m_Str.c_str());
		}

		tempData->m_ArmorUpgradeCode = -1;
		m_ParseHeroData->m_Armor.push_back(tempData);
	}

	for (int i = 0; i < 4; i++)
	{
		sizeIndex = enterCount[7 + i];
		end = enterCount[8 + i];
		indexStr = innerString.GetCutText(sizeIndex + 2, end - 1);
		HeroData::HeroArmor* tempData = new HeroData::HeroArmor();

		{
			sizeIndex = indexStr.Find(L".name");
			end = indexStr.Find(L".def");
			subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
			tempData->m_Name = subIndexStr;
		}

		{
			sizeIndex = indexStr.Find(L".def");
			end = indexStr.Find(L".prot");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
			tempData->m_Def = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".prot");
			end = indexStr.Find(L".hp");
			subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 1);
			tempData->m_Prot = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".hp");
			end = indexStr.Find(L".spd");
			subIndexStr = indexStr.GetCutText(sizeIndex + 4, end - 2);
			tempData->m_Hp = std::stof(subIndexStr.m_Str.c_str());
		}

		{
			sizeIndex = indexStr.Find(L".spd");
			end = indexStr.Find(L".upgradeRequirementCode");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 1);
			tempData->m_Speed = std::stof(subIndexStr.m_Str.c_str());
		}

		tempData->m_ArmorUpgradeCode = i;
		m_ParseHeroData->m_Armor.push_back(tempData);
	}

	//skill
	{
		for (int i = 0; i < 4; i++)
		{
			readSkill(11 + i * 5, innerString, &enterCount);
		}

		{
			std::vector<HeroData::HeroSkill*> tempVector;
			HeroData::HeroSkill* tempData = new HeroData::HeroSkill();

			sizeIndex = enterCount[31];
			end = enterCount[32];
			indexStr = innerString.GetCutText(sizeIndex + 5, end + 1);

			{
				sizeIndex = indexStr.Find(L".id");
				end = indexStr.Find(L".level");
				subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
				tempData->m_Name = subIndexStr;
			}

			tempData->m_SkillLevel = 0;

			{
				sizeIndex = indexStr.Find(L".move");
				end = indexStr.Find(L".launch");
				subIndexStr = indexStr.GetCutText(sizeIndex + 6, sizeIndex + 6);
				tempData->m_MoveEffect[0] = std::stoi(subIndexStr.m_Str.c_str());
				subIndexStr = indexStr.GetCutText(sizeIndex + 8, sizeIndex + 8);
				tempData->m_MoveEffect[1] = std::stoi(subIndexStr.m_Str.c_str());
			}

			{
				sizeIndex = indexStr.Find(L".launch");
				end = indexStr.Find(L"\r\n");
				size_t size = end - sizeIndex;
				size -= 8;

				for (size_t i = 0; i < size; i++)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
					tempData->m_LaunchPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
				}
			}

			tempVector.push_back(tempData);

			std::pair<std::map<GameString, std::vector<HeroData::HeroSkill*>>::iterator, bool> temp;
			temp = m_ParseHeroData->m_Skill.insert(std::map<GameString, std::vector<HeroData::HeroSkill*>>::value_type(tempVector[0]->m_Name, tempVector));
			if (!temp.second)
			{
				ASSERTMSG;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			readSkill(32 + i * 5, innerString, &enterCount);
		}

		//HighWayman 전용 riposte
		{
			sizeIndex = innerString.Find(L"riposte_skill: .id");
			end = innerString.Find(L"tag: ");
			if (sizeIndex != std::wstring::npos)
			{
				std::vector<HeroData::HeroSkill*> tempVector;
				HeroData::HeroSkill* tempData = new HeroData::HeroSkill();

				indexStr = innerString.GetCutText(sizeIndex + 15, end - 1);

				{
					sizeIndex = indexStr.Find(L".id");
					end = indexStr.Find(L".level");
					subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
					tempData->m_Name = subIndexStr;
				}

				tempData->m_SkillLevel = 0;

				{
					sizeIndex = indexStr.Find(L".type");
					end = indexStr.Find(L".atk");
					subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
					tempData->m_SkillType = CheckSkillType(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".atk");
					end = indexStr.Find(L".dmg");
					subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
					tempData->m_ATK = std::stof(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".dmg");
					end = indexStr.Find(L".crit");
					subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
					tempData->m_DmgMod = std::stof(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".crit");
					end = indexStr.Find(L".launch");
					subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
					tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".launch");
					end = indexStr.Find(L".target");
					size_t size = end - sizeIndex;
					size -= 9;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						tempData->m_LaunchPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
					}
				}

				{
					sizeIndex = indexStr.Find(L".target");
					end = indexStr.Find(L".is_crit_valid");

					size_t size = end - sizeIndex;
					size -= 9;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						if (subIndexStr.m_Str[0] == L'~')
						{
							tempData->m_AOE = true;
							continue;
						}
						tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
					}

					subIndexStr = indexStr.GetCutText(end + 15, end + 18);
					if (subIndexStr[0] == L'T')
					{
						tempData->m_Is_Crit_Valid = true;
					}

					tempVector.push_back(tempData);

					std::pair<std::map<GameString, std::vector<HeroData::HeroSkill*>>::iterator, bool> temp;
					temp = m_ParseHeroData->m_Skill.insert(std::map<GameString, std::vector<HeroData::HeroSkill*>>::value_type(tempVector[0]->m_Name, tempVector));
					if (!temp.second)
					{
						ASSERTMSG;
					}
				}
			}

		} // end Skill
	}

	Actor()->GetCom<SHEROCOMBAT>()->m_HeroData = m_ParseHeroData;
}


void TextReader::readSkill(int _StartLine, GameString& _InnerStr, std::vector<size_t>* _enterCount)
{
	size_t sizeIndex;
	size_t end;
	GameString indexStr;
	GameString subIndexStr;

	std::vector<HeroData::HeroSkill*> tempVector;

	for (int i = 0; i < 5; ++i)
	{
		sizeIndex = (*_enterCount)[_StartLine + i];
		end = (*_enterCount)[_StartLine + 1 + i];
		indexStr = _InnerStr.GetCutText(sizeIndex + 2, end + 1);
		HeroData::HeroSkill* tempData = new HeroData::HeroSkill();

		{
			sizeIndex = indexStr.Find(L".id");
			end = indexStr.Find(L".level");
			subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
			tempData->m_Name = subIndexStr;
		}

		{
			tempData->m_SkillLevel = i;
		}

		{
			sizeIndex = indexStr.Find(L".type");
			end = indexStr.Find(L".atk");
			if (sizeIndex != std::wstring::npos)
			{
				subIndexStr = indexStr.GetCutText(sizeIndex + 7, end - 3);
				tempData->m_SkillType = CheckSkillType(subIndexStr.m_Str.c_str());


				{
					sizeIndex = indexStr.Find(L".atk");
					end = indexStr.Find(L".dmg");
					subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
					tempData->m_ATK = std::stof(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".dmg");
					end = indexStr.Find(L".crit");
					subIndexStr = indexStr.GetCutText(sizeIndex + 5, end - 3);
					tempData->m_DmgMod = std::stof(subIndexStr.m_Str.c_str());
				}

				{
					sizeIndex = indexStr.Find(L".crit");
					end = indexStr.Find(L".move");
					if (std::wstring::npos != end)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
						tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
						tempData->m_bIsMoveable = true;

						sizeIndex = indexStr.Find(L".move");
						end = indexStr.Find(L".launch");
						subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
						tempData->m_MoveEffect[0] = std::stoi(subIndexStr.m_Str.c_str());
						subIndexStr = indexStr.GetCutText(sizeIndex + 8, sizeIndex + 8);
						tempData->m_MoveEffect[1] = std::stoi(subIndexStr.m_Str.c_str());
					}
					else if (std::wstring::npos != indexStr.Find(L".per_battle_limit"))
					{
						end = indexStr.Find(L".per_battle_limit");
						subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
						tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());

						sizeIndex = indexStr.Find(L".per_battle_limit");
						end = indexStr.Find(L".launch");
						subIndexStr = indexStr.GetCutText(sizeIndex + 18, end - 2);
						tempData->m_Per_Battle_Limit = std::stoi(subIndexStr.m_Str.c_str());
					}
					else
					{
						end = indexStr.Find(L".launch");
						subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
						tempData->m_Crit = std::stof(subIndexStr.m_Str.c_str());
					}
				}
			}
			else if (indexStr.Find(L".heal") != std::wstring::npos)
			{
				tempData->m_CanHeal = true;
				sizeIndex = indexStr.Find(L".heal");
				end = indexStr.Find(L".launch");
				subIndexStr = indexStr.GetCutText(sizeIndex + 6, end - 3);
				tempData->m_HealMin = std::stoi(subIndexStr.m_Str.c_str());
				subIndexStr = indexStr.GetCutText(sizeIndex + 8, sizeIndex + 8);
				tempData->m_HealMax = std::stoi(subIndexStr.m_Str.c_str());
			}
			else
			{
				ASSERTMSG;
			}
		}


		{
			sizeIndex = indexStr.Find(L".launch");
			end = indexStr.Find(L".target");
			size_t size = end - sizeIndex;
			size -= 9;

			for (size_t i = 0; i < size; i++)
			{
				subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
				tempData->m_LaunchPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
			}
		}

		{
			sizeIndex = indexStr.Find(L".target");
			end = indexStr.Find(L".is_crit_valid");

			if (std::wstring::npos != end)
			{
				size_t size = end - sizeIndex;
				size -= 9;

				for (size_t i = 0; i < size; i++)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
					if (subIndexStr.m_Str[0] == L'~')
					{
						tempData->m_AOE = true;
						continue;
					}
					else if (subIndexStr.m_Str[0] == L'@')
					{
						tempData->m_IsBenefitSkill = true;
						continue;
					}
					tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
				}

				subIndexStr = indexStr.GetCutText(end + 15, end + 18);
				if (subIndexStr[0] == L'T')
				{
					tempData->m_Is_Crit_Valid = true;
				}
			}
			else if (std::wstring::npos != indexStr.Find(L".effect"))
			{

				end = indexStr.Find(L".effect");

				size_t size = end - sizeIndex;
				size -= 10;

				for (size_t i = 0; i < size; i++)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
					if (subIndexStr.m_Str[0] == L'~')
					{
						tempData->m_AOE = true;
						continue;
					}
					else if (subIndexStr.m_Str[0] == L'@')
					{
						tempData->m_IsBenefitSkill = true;
						continue;
					}
					tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
				}
			}
			else if (std::wstring::npos != indexStr.Find(L".generation_guaranteed"))
			{
				end = indexStr.Find(L".generation_guaranteed");
				tempData->m_Generation_guarantee = true;

				size_t size = end - sizeIndex;
				size -= 10;

				for (size_t i = 0; i < size; i++)
				{
					subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
					if (subIndexStr.m_Str[0] == L'~')
					{
						tempData->m_AOE = true;
						continue;
					}
					else if (subIndexStr.m_Str[0] == L'@')
					{
						tempData->m_IsBenefitSkill = true;
						continue;
					}
					tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
				}
			}
			else
			{
				end = indexStr.Find(L".is_stall_invalidating");
				if (end != std::wstring::npos)
				{
					tempData->m_Is_Stall_Invalidating = false;

					size_t size = end - sizeIndex;
					size -= 10;

					for (size_t i = 0; i < size; i++)
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 8 + i, sizeIndex + 8 + i);
						if (subIndexStr.m_Str[0] == L'~')
						{
							tempData->m_AOE = true;
							continue;
						}
						else if (subIndexStr.m_Str[0] == L'@')
						{
							tempData->m_IsBenefitSkill = true;
							continue;
						}
						tempData->m_TargetPos[std::stoi(subIndexStr.m_Str.c_str()) - 1] = true;
					}
				}
			}

		}

		{
			sizeIndex = indexStr.Find(L".effect");
			if (std::wstring::npos != sizeIndex)
			{
				if (std::wstring::npos != indexStr.Find(L".generation_guaranteed"))
				{
					tempData->m_Generation_guarantee = true;
					end = indexStr.Find(L".generation_guaranteed");
					subIndexStr = indexStr.GetCutText(sizeIndex + 9, end - 3);

					GameString twoSubIndexStr;

					std::vector<size_t> skillCount = subIndexStr.FindToCount(L"\" \"");
					if (skillCount.size() != 0)
					{
						size_t count = skillCount.size();

						end = skillCount[0];
						twoSubIndexStr = subIndexStr.GetCutText(0, end - 1);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());

						for (size_t i = 0; i < count - 1; i++)
						{
							sizeIndex = skillCount[0 + i];
							end = skillCount[1 + i];
							twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 1);
							tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
						}

						sizeIndex = skillCount[count - 1];
						end = subIndexStr.Find(L".launch");
						twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 3);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
					}
					else
					{
						tempData->m_Effect.push_back(subIndexStr.m_Str.c_str());
					}
				}
				else
				{
					end = indexStr.Find(L"\r\n");
					subIndexStr = indexStr.GetCutText(sizeIndex + 9, end + 1);

					GameString twoSubIndexStr;

					std::vector<size_t> skillCount = subIndexStr.FindToCount(L"\" \"");
					if (skillCount.size() != 0)
					{
						size_t count = skillCount.size();

						end = skillCount[0];
						twoSubIndexStr = subIndexStr.GetCutText(0, end - 1);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());

						for (size_t i = 0; i < count - 1; i++)
						{
							sizeIndex = skillCount[0 + i];
							end = skillCount[1 + i];
							twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 1);
							tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
						}

						sizeIndex = skillCount[count - 1];
						end = subIndexStr.Find(L"\r\n");
						twoSubIndexStr = subIndexStr.GetCutText(sizeIndex + 3, end - 3);
						tempData->m_Effect.push_back(twoSubIndexStr.m_Str.c_str());
					}
					else
					{
						subIndexStr = indexStr.GetCutText(sizeIndex + 9, end - 2);
						tempData->m_Effect.push_back(subIndexStr.m_Str.c_str());
					}

				}
			}

		}


		{
			sizeIndex = indexStr.Find(L".generation_guaranteed ");

			if (sizeIndex != std::wstring::npos)
			{
				tempData->m_Generation_guarantee = true;
			}
		}


		tempVector.push_back(tempData);
		if (i == 4)
		{
			std::pair<std::map<GameString, std::vector<HeroData::HeroSkill*>>::iterator, bool> temp;
			temp = m_ParseHeroData->m_Skill.insert(std::map<GameString, std::vector<HeroData::HeroSkill*>>::value_type(tempVector[0]->m_Name, tempVector));
			if (!temp.second)
			{
				ASSERTMSG;
			}
		}
	}
}

bool TextReader::LoadBinaryData(GameString& _Name, bool b_Is_Monster)
{
	bool b_Can_Open = true;
	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	if (false == m_Dir.TryFindFileRecursive(L"Data"))
	{
		ASSERTMSG;
	}

	int dataCount = 0;
	GameFile loadFile = m_Dir.CreateGameFileReadOnly(_Name + L"_binary.Data");

	if (false == loadFile.Open())
	{
		perror("open error");
		b_Can_Open = false;
		goto fileClose;
	}

	if (b_Is_Monster)
	{
		if (nullptr != m_ParseData)
		{
			delete m_ParseData;
		}
		m_ParseData = new MonsterData();

		loadFile.Read(m_ParseData->m_DisplaySize);
		loadFile.Read(m_ParseData->m_MonsterType);
		loadFile.Read(m_ParseData->m_Status);
		loadFile.Read(dataCount);

		m_ParseData->m_Skill.reserve(dataCount);

		for (int i = 0; i < dataCount; i++)
		{
			CombatSkill* tempSkill = new CombatSkill();
			loadFile.Read(tempSkill->m_Name);
			loadFile.Read(tempSkill->m_ATK);
			loadFile.Read(tempSkill->m_SkillType);
			loadFile.Read(tempSkill->m_DmgMin);
			loadFile.Read(tempSkill->m_DmgMax);
			loadFile.Read(tempSkill->m_Crit);
			loadFile.Read(tempSkill->m_bIsMoveable);
			loadFile.Read(tempSkill->m_CanHeal);
			loadFile.Read(tempSkill->m_MoveEffect[0]);
			loadFile.Read(tempSkill->m_MoveEffect[1]);
			loadFile.Read(tempSkill->m_HealMin);
			loadFile.Read(tempSkill->m_HealMax);
			loadFile.Read(tempSkill->m_LaunchPos[0]);
			loadFile.Read(tempSkill->m_LaunchPos[1]);
			loadFile.Read(tempSkill->m_LaunchPos[2]);
			loadFile.Read(tempSkill->m_LaunchPos[3]);
			loadFile.Read(tempSkill->m_TargetPos[0]);
			loadFile.Read(tempSkill->m_TargetPos[1]);
			loadFile.Read(tempSkill->m_TargetPos[2]);
			loadFile.Read(tempSkill->m_TargetPos[3]);
			loadFile.Read(tempSkill->m_AOE);
			int effectCount = 0;
			loadFile.Read(effectCount);
			for (int i = 0; i < effectCount; i++)
			{
				GameString Effect;
				loadFile.Read(Effect);
				tempSkill->m_Effect.push_back(Effect);
			}

			m_ParseData->m_Skill.push_back(*tempSkill);
			delete tempSkill;
		}

		Actor()->GetCom<SMONSTERCOMBAT>()->m_MonsterData = *m_ParseData;
	}
	else
	{
		if (nullptr != m_ParseHeroData)
		{
			delete m_ParseHeroData;
		}
		m_ParseHeroData = new HeroData();
		loadFile.Read(m_ParseHeroData->m_Status);
		loadFile.Read(dataCount);
		for (int i = 0; i < dataCount; i++)
		{
			HeroData::HeroArmor* newArmor = new HeroData::HeroArmor;
			loadFile.Read(newArmor->m_Name);
			loadFile.Read(newArmor->m_Def);
			loadFile.Read(newArmor->m_Prot);
			loadFile.Read(newArmor->m_Hp);
			loadFile.Read(newArmor->m_Speed);
			loadFile.Read(newArmor->m_ArmorUpgradeCode);
			m_ParseHeroData->m_Armor.push_back(newArmor);
		}

		loadFile.Read(dataCount);
		for (int i = 0; i < dataCount; i++)
		{
			HeroData::HeroWeapon* newWeapon = new HeroData::HeroWeapon;
			loadFile.Read(newWeapon->m_Name);
			loadFile.Read(newWeapon->m_Atk);
			loadFile.Read(newWeapon->m_MinDmg);
			loadFile.Read(newWeapon->m_MaxDmg);
			loadFile.Read(newWeapon->m_Crit);
			loadFile.Read(newWeapon->m_Speed);
			loadFile.Read(newWeapon->m_WeaponUpgradeCode);
			m_ParseHeroData->m_Weapon.push_back(newWeapon);
		}

		loadFile.Read(m_ParseHeroData->m_Effect);

		int skillCount;
		loadFile.Read(skillCount);

		//Hero skill은 Skillvector를 먼저 채우고 그 다음에 이름과 같이 map에 저장해야한다.
		GameString skillName;

		std::vector<HeroData::HeroSkill*> skillVector;
		for (int j = 0; j < skillCount; j++)
		{
			loadFile.Read(skillName);
			loadFile.Read(dataCount);
			for (int i = 0; i < dataCount; i++)
			{
				HeroData::HeroSkill* newSkill = new HeroData::HeroSkill;
				loadFile.Read(newSkill->m_Name);
				loadFile.Read(newSkill->m_ATK);
				loadFile.Read(newSkill->m_SkillType);
				loadFile.Read(newSkill->m_DmgMin);
				loadFile.Read(newSkill->m_DmgMax);
				loadFile.Read(newSkill->m_Crit);
				loadFile.Read(newSkill->m_bIsMoveable);
				loadFile.Read(newSkill->m_CanHeal);
				loadFile.Read(newSkill->m_MoveEffect[0]);
				loadFile.Read(newSkill->m_MoveEffect[1]);
				loadFile.Read(newSkill->m_HealMin);
				loadFile.Read(newSkill->m_HealMax);
				loadFile.Read(newSkill->m_LaunchPos[0]);
				loadFile.Read(newSkill->m_LaunchPos[1]);
				loadFile.Read(newSkill->m_LaunchPos[2]);
				loadFile.Read(newSkill->m_LaunchPos[3]);
				loadFile.Read(newSkill->m_TargetPos[0]);
				loadFile.Read(newSkill->m_TargetPos[1]);
				loadFile.Read(newSkill->m_TargetPos[2]);
				loadFile.Read(newSkill->m_TargetPos[3]);
				loadFile.Read(newSkill->m_AOE);

				//Hero Only 속성
				loadFile.Read(newSkill->m_Generation_guarantee);
				loadFile.Read(newSkill->m_Is_Stall_Invalidating);
				loadFile.Read(newSkill->m_IsBenefitSkill);
				loadFile.Read(newSkill->m_Is_Crit_Valid);
				loadFile.Read(newSkill->m_Per_Battle_Limit);
				loadFile.Read(newSkill->m_SkillLevel);
				loadFile.Read(newSkill->m_DmgMod);
				skillVector.push_back(newSkill);

				int effectCount = 0;
				loadFile.Read(effectCount);
				for (int i = 0; i < effectCount; i++)
				{
					GameString Effect;
					loadFile.Read(Effect);
					newSkill->m_Effect.push_back(Effect);
				}
			}
			m_ParseHeroData->m_Skill.insert(std::map<GameString, std::vector<HeroData::HeroSkill*>>::value_type(skillName, skillVector));
			skillVector.clear();
		}
		m_ParseHeroData = m_ParseHeroData;
		Actor()->GetCom<SHEROCOMBAT>()->m_HeroData = m_ParseHeroData;
	}

fileClose:
	if (false == loadFile.Close())
	{
		perror("close Error");
		return false;
	}
	if (!b_Can_Open)
	{
		return false;
	}
	return true;
}

void TextReader::SaveBinaryData(GameString& _Name)
{
	if (nullptr == m_ParseData && nullptr == m_ParseHeroData)
	{
		ASSERTMSG;
	}

	//몬스터 데이터 바이너리 저장
	if (nullptr != m_ParseData)
	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		if (false == m_Dir.TryFindFileRecursive(L"Data"))
		{
			ASSERTMSG;
		}

		GameFile saveFile = m_Dir.CreateGameFile(_Name + L"_binary.Data");
		saveFile.Open();
		saveFile.Write(m_ParseData->m_DisplaySize);
		saveFile.Write(m_ParseData->m_MonsterType);
		saveFile.Write(m_ParseData->m_Status);

		saveFile.Write((int)m_ParseData->m_Skill.size());
		for (int i = 0; i < m_ParseData->m_Skill.size(); ++i)
		{
			saveFile.Write(m_ParseData->m_Skill[i].m_Name);
			saveFile.Write(m_ParseData->m_Skill[i].m_ATK);
			saveFile.Write(m_ParseData->m_Skill[i].m_SkillType);
			saveFile.Write(m_ParseData->m_Skill[i].m_DmgMin);
			saveFile.Write(m_ParseData->m_Skill[i].m_DmgMax);
			saveFile.Write(m_ParseData->m_Skill[i].m_Crit);
			saveFile.Write(m_ParseData->m_Skill[i].m_bIsMoveable);
			saveFile.Write(m_ParseData->m_Skill[i].m_CanHeal);
			saveFile.Write(m_ParseData->m_Skill[i].m_MoveEffect[0]);
			saveFile.Write(m_ParseData->m_Skill[i].m_MoveEffect[1]);
			saveFile.Write(m_ParseData->m_Skill[i].m_HealMin);
			saveFile.Write(m_ParseData->m_Skill[i].m_HealMax);
			saveFile.Write(m_ParseData->m_Skill[i].m_LaunchPos[0]);
			saveFile.Write(m_ParseData->m_Skill[i].m_LaunchPos[1]);
			saveFile.Write(m_ParseData->m_Skill[i].m_LaunchPos[2]);
			saveFile.Write(m_ParseData->m_Skill[i].m_LaunchPos[3]);
			saveFile.Write(m_ParseData->m_Skill[i].m_TargetPos[0]);
			saveFile.Write(m_ParseData->m_Skill[i].m_TargetPos[1]);
			saveFile.Write(m_ParseData->m_Skill[i].m_TargetPos[2]);
			saveFile.Write(m_ParseData->m_Skill[i].m_TargetPos[3]);
			saveFile.Write(m_ParseData->m_Skill[i].m_AOE);

			saveFile.Write((int)m_ParseData->m_Skill[i].m_Effect.size());
			std::list<GameString>::iterator startIter = m_ParseData->m_Skill[i].m_Effect.begin();
			std::list<GameString>::iterator endIter = m_ParseData->m_Skill[i].m_Effect.end();

			for (; startIter != endIter; ++startIter)
			{
				saveFile.Write(*startIter);
			}

		}

		saveFile.Close();

	}
	//히어로 데이터 저장
	else
	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework"); 
		if (false == m_Dir.TryFindFileRecursive(L"Data"))
		{
			ASSERTMSG;
		}

		GameFile saveFile = m_Dir.CreateGameFile(_Name + L"_binary.Data");
		saveFile.Open();
		saveFile.Write(m_ParseHeroData->m_Status);
		int DataCount = (int)m_ParseHeroData->m_Armor.size();
		saveFile.Write(DataCount);
		for (int i = 0; i < DataCount; i++)
		{
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_Name);
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_Def);
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_Prot);
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_Hp);
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_Speed);
			saveFile.Write(m_ParseHeroData->m_Armor[i]->m_ArmorUpgradeCode);
		}

		DataCount = (int)m_ParseHeroData->m_Weapon.size();
		saveFile.Write(DataCount);
		for (int i = 0; i < DataCount; i++)
		{
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_Name);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_Atk);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_MinDmg);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_MaxDmg);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_Crit);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_Speed);
			saveFile.Write(m_ParseHeroData->m_Weapon[i]->m_WeaponUpgradeCode);
		}

		saveFile.Write(m_ParseHeroData->m_Effect);

		DataCount = (int)m_ParseHeroData->m_Skill.size();
		saveFile.Write(DataCount);

		std::map<GameString, std::vector<HeroData::HeroSkill*>>::iterator StartIter = m_ParseHeroData->m_Skill.begin();
		std::map<GameString, std::vector<HeroData::HeroSkill*>>::iterator EndIter = m_ParseHeroData->m_Skill.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			//스킬명 저장
			saveFile.Write(StartIter->first);
			DataCount = (int)StartIter->second.size();
			saveFile.Write(DataCount);
			//스킬 레벨당 5개 저장
			for (int i = 0; i < DataCount; i++)
			{
				saveFile.Write(StartIter->second[i]->m_Name);
				saveFile.Write(StartIter->second[i]->m_ATK);
				saveFile.Write(StartIter->second[i]->m_SkillType);
				saveFile.Write(StartIter->second[i]->m_DmgMin);
				saveFile.Write(StartIter->second[i]->m_DmgMax);
				saveFile.Write(StartIter->second[i]->m_Crit);
				saveFile.Write(StartIter->second[i]->m_bIsMoveable);
				saveFile.Write(StartIter->second[i]->m_CanHeal);
				saveFile.Write(StartIter->second[i]->m_MoveEffect[0]);
				saveFile.Write(StartIter->second[i]->m_MoveEffect[1]);
				saveFile.Write(StartIter->second[i]->m_HealMin);
				saveFile.Write(StartIter->second[i]->m_HealMax);
				saveFile.Write(StartIter->second[i]->m_LaunchPos[0]);
				saveFile.Write(StartIter->second[i]->m_LaunchPos[1]);
				saveFile.Write(StartIter->second[i]->m_LaunchPos[2]);
				saveFile.Write(StartIter->second[i]->m_LaunchPos[3]);
				saveFile.Write(StartIter->second[i]->m_TargetPos[0]);
				saveFile.Write(StartIter->second[i]->m_TargetPos[1]);
				saveFile.Write(StartIter->second[i]->m_TargetPos[2]);
				saveFile.Write(StartIter->second[i]->m_TargetPos[3]);
				saveFile.Write(StartIter->second[i]->m_AOE);

				//Hero Only 속성
				saveFile.Write(StartIter->second[i]->m_Generation_guarantee);
				saveFile.Write(StartIter->second[i]->m_Is_Stall_Invalidating);
				saveFile.Write(StartIter->second[i]->m_IsBenefitSkill);
				saveFile.Write(StartIter->second[i]->m_Is_Crit_Valid);
				saveFile.Write(StartIter->second[i]->m_Per_Battle_Limit);
				saveFile.Write(StartIter->second[i]->m_SkillLevel);
				saveFile.Write(StartIter->second[i]->m_DmgMod);

				saveFile.Write((int)StartIter->second[i]->m_Effect.size());
				std::list<GameString>::iterator startIter = StartIter->second[i]->m_Effect.begin();
				std::list<GameString>::iterator endIter = StartIter->second[i]->m_Effect.end();

				for (; startIter != endIter; ++startIter)
				{
					saveFile.Write(*startIter);
				}
			}
		}

		saveFile.Close();
	}

}