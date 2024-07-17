#include "Combat.h"

#include <GameAni.h>
#include <GameDic.h>
#include <GameTexture.h>
#include <GameSprite.h>
#include <GameImage.h>
#include <GameTime.h>
#include <GameFont.h>
#include <GameDebugEn.h>
#include <GameCam.h>

#include "DungeonScene.h"
#include "GameJsonReader.h"

bool compare(std::pair<float, GameActor*> _left, std::pair<float, GameActor*> _right);

const GameString SGAMECOMBAT::m_ArmorSpriteName[5] = { L"eqp_armour_0.png", L"eqp_armour_1.png", L"eqp_armour_2.png" , L"eqp_armour_3.png", L"eqp_armour_4.png" };
const GameString SGAMECOMBAT::m_WepaonSpriteName[5] = { L"eqp_weapon_0.png", L"eqp_weapon_1.png", L"eqp_weapon_2.png" , L"eqp_weapon_3.png", L"eqp_weapon_4.png" };
const GameString SGAMECOMBAT::m_SkillName[7] = { L".ability.one.png", L".ability.two.png", L".ability.three.png" , L".ability.four.png", L".ability.five.png", L".ability.six.png", L".ability.seven.png" };
const GameString SGAMECOMBAT::m_BlackSkillName[7] = { L".ability.one.black.png", L".ability.two.black.png", L".ability.three.black.png" , L".ability.four.black.png", L".ability.five.black.png", L".ability.six.black.png", L".ability.seven.black.png" };
const CVector SGAMECOMBAT::m_AbilityPos[4] = { {-2.76f, -1.64f, 7.9f, 1.0f}, {-2.23f, -1.64f, 7.9f, 1.0f}, {-1.72f, -1.64f, 7.9f, 1.0f}, {-1.2f, -1.64f, 7.9f, 1.0f} };

SGAMECOMBAT::SGAMECOMBAT() : m_HeroCount(0), m_MonsterCount(0), m_SelectUITimer(0), b_Is_Battle(false)
{

}


SGAMECOMBAT::~SGAMECOMBAT()
{
}

void SGAMECOMBAT::TurnUpdate()
{
	for (auto& pair : m_CurCombat->second->m_SpeedArrange)
	{
		m_CurCombat->second->m_SpeedStack.push(pair.second);
	}
}

void SGAMECOMBAT::Init()
{
	//select
	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		m_Dir.Move(L"res");
		if (!m_Dir.TryFindFileRecursive(L"overlays"))
		{
			ASSERTMSG;
		}
		GameTexture::Load(m_Dir.CreateGameFilePath(L"select1.png"));
		m_SelectSprite[0] = GameSprite::Create(m_Dir.CreateGameFilePath(L"select1.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"select2.png"));
		m_SelectSprite[1] = GameSprite::Create(m_Dir.CreateGameFilePath(L"select2.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"select3.png"));
		m_SelectSprite[2] = GameSprite::Create(m_Dir.CreateGameFilePath(L"select3.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"select4.png"));
		m_SelectSprite[3] = GameSprite::Create(m_Dir.CreateGameFilePath(L"select4.png"), 1, 1);

		GameTexture::Load(m_Dir.CreateGameFilePath(L"target_1.png"));
		m_TargetSprite[0] = GameSprite::Create(m_Dir.CreateGameFilePath(L"target_1.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"target_2.png"));
		m_TargetSprite[1] = GameSprite::Create(m_Dir.CreateGameFilePath(L"target_2.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"target_3.png"));
		m_TargetSprite[2] = GameSprite::Create(m_Dir.CreateGameFilePath(L"target_3.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"target_4.png"));
		m_TargetSprite[3] = GameSprite::Create(m_Dir.CreateGameFilePath(L"target_4.png"), 1, 1);

		m_CombatActor = CreateActor();

		m_SelectRender = m_CombatActor->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PreMapUI);
		m_SelectRender->Sprite(m_SelectSprite[0]);
		// (k, k* 2.06f / 1.7f)
		m_SelectRender->Trans()->WSCALE({ 1.0f, 1.2f, 0.0f, 1.0f });

		m_MaxSelectUIWidth = 1.4f;
		m_MinSelectUIWidth = 1.0f;

		m_SelectUITimer = 1.25f;
	}

	//Combat UI초기화
	{
		GameDic m_Dir;
		m_Dir.MoveParent(L"GameFramework");
		m_Dir.Move(L"res");
		if (!m_Dir.TryFindFileRecursive(L"icons_equip"))
		{
			ASSERTMSG;
		}
		GameString tempStr;

		GameTexture::Load(m_Dir.CreateGameFilePath(L"ability_move.png"));
		m_MoveSprite = GameSprite::Create(m_Dir.CreateGameFilePath(L"ability_move.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"ability_pass.png"));
		m_AbilityPassSprite = GameSprite::Create(m_Dir.CreateGameFilePath(L"ability_pass.png"), 1, 1);
		GameTexture::Load(m_Dir.CreateGameFilePath(L"selected_ability.png"));
		m_SelectAbilityprite = GameSprite::Create(m_Dir.CreateGameFilePath(L"selected_ability.png"), 1, 1);

		//HeroArmor,Weapon 초기화
		for (int i = 0; i < 4; i++)
		{
			if (false == m_Dir.Move(ActorList::PlayerList[i]))
			{
				ASSERTMSG;
			}

			//sprite 불러놓기
			for (int j = 0; j < 5; j++)
			{
				tempStr = ActorList::PlayerList[i] + L"_";
				tempStr += m_ArmorSpriteName[j];
				GameTexture::Load(m_Dir.CreateGameFilePath(tempStr));
				m_HeroUISprite[i].m_HeroArmor[j] = GameSprite::Create(m_Dir.CreateGameFilePath(tempStr), 1, 1);

			}

			for (int j = 0; j < 5; j++)
			{
				tempStr = ActorList::PlayerList[i] + L"_";
				tempStr += m_WepaonSpriteName[j];
				GameTexture::Load(m_Dir.CreateGameFilePath(tempStr));
				m_HeroUISprite[i].m_HeroWeapon[j] = GameSprite::Create(m_Dir.CreateGameFilePath(tempStr), 1, 1);
			}

			for (int j = 0; j < 7; j++)
			{
				tempStr = ActorList::PlayerList[i];
				tempStr += m_SkillName[j];
				GameTexture::Load(m_Dir.CreateGameFilePath(tempStr));
				m_HeroUISprite[i].m_HeroAbility[j] = GameSprite::Create(m_Dir.CreateGameFilePath(tempStr), 1, 1);
			}

			for (int j = 0; j < 7; j++)
			{
				tempStr = ActorList::PlayerList[i];
				tempStr += m_BlackSkillName[j];
				GameTexture::Load(m_Dir.CreateGameFilePath(tempStr));
				m_HeroUISprite[i].m_HeroAbilityBlack[j] = GameSprite::Create(m_Dir.CreateGameFilePath(tempStr), 1, 1);
			}

			tempStr = ActorList::PlayerList[i] + L"_";
			tempStr += L"portrait_roster.png";
			GameTexture::Load(m_Dir.CreateGameFilePath(tempStr));
			m_HeroUISprite[i].m_HeroPortrait = GameSprite::Create(m_Dir.CreateGameFilePath(tempStr), 1, 1);

			m_Dir.MoveParent();
		}

		//Renderer 추가하기
		{
			m_HeroArmorRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroWeaponRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroAbilityRender[0] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroAbilityRender[1] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroAbilityRender[2] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroAbilityRender[3] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_MoveSpritRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_AbilityPassSpriteRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_HeroPortraitRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_UI);
			m_SelectAbilitypriteRender = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PostUI);
			//m_SelectAbilitypriteRender->Off();
		}


		//TargetRender 추가하기
		{
			m_TargetRender[0] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PostUI);
			m_TargetRender[1] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PostUI);
			m_TargetRender[2] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PostUI);
			m_TargetRender[3] = Actor()->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_PostUI);
			m_TargetRender[0]->Off();
			m_TargetRender[1]->Off();
			m_TargetRender[2]->Off();
			m_TargetRender[3]->Off();
		}


	}
}

void SGAMECOMBAT::PrintHeroUI(int _HeroIndex/* = 0*/)
{
	m_CurHeroCombat = m_HeroActor[_HeroIndex]->GetCom<SHEROCOMBAT>();

	m_HeroArmorRender->Sprite(m_HeroUISprite[_HeroIndex].m_HeroArmor[m_CurHeroCombat->m_HeroArmorIndex]);
	m_HeroArmorRender->WPOS({ -2.2f, -2.89f, 8.0f, 1.0f });
	m_HeroArmorRender->WSCALE({ 0.5f, 0.92f });
	m_HeroWeaponRender->Sprite(m_HeroUISprite[_HeroIndex].m_HeroWeapon[m_CurHeroCombat->m_HeroArmorIndex]);
	m_HeroWeaponRender->WPOS({ -2.8f, -2.89f, 8.0f, 1.0f });
	m_HeroWeaponRender->WSCALE({ 0.5f, 0.92f });
	m_MoveSpritRender->Sprite(m_MoveSprite);
	m_MoveSpritRender->WPOS({ -0.7f, -1.64f, 8.0f, 1.0f });
	m_MoveSpritRender->WSCALE({ 0.5f, 0.55f });
	m_AbilityPassSpriteRender->Sprite(m_AbilityPassSprite);
	m_AbilityPassSpriteRender->WPOS({ -0.36f, -1.64f, 8.0f, 1.0f });
	m_AbilityPassSpriteRender->WSCALE({ 0.20f, 0.55f });
	m_HeroPortraitRender->Sprite(m_HeroUISprite[_HeroIndex].m_HeroPortrait);
	m_HeroPortraitRender->WPOS({ -4.35f, -1.7f, 8.0f, 1.0f });
	m_HeroPortraitRender->WSCALE({ 0.6f, 0.6f });
	m_SelectAbilitypriteRender->Sprite(m_SelectAbilityprite);
	m_SelectAbilitypriteRender->WSCALE({ 0.74f, 0.78f });
	m_SelectAbilitypriteRender->WPOS(m_AbilityPos[m_SelectSkillIndex]);

	m_HeroAbilityRender[0]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbility[0]);
	m_HeroAbilityRender[0]->WPOS({ -2.76f, -1.64f, 8.0f, 1.0f });
	m_HeroAbilityRender[0]->WSCALE({ 0.52f, 0.55f });
	m_HeroAbilityRender[1]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbility[1]);
	m_HeroAbilityRender[1]->WPOS({ -2.23f, -1.64f, 8.0f, 1.0f });
	m_HeroAbilityRender[1]->WSCALE({ 0.52f, 0.55f });
	m_HeroAbilityRender[2]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbility[2]);
	m_HeroAbilityRender[2]->WPOS({ -1.72f, -1.64f, 8.0f, 1.0f });
	m_HeroAbilityRender[2]->WSCALE({ 0.52f, 0.55f });
	m_HeroAbilityRender[3]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbility[3]);
	m_HeroAbilityRender[3]->WPOS({ -1.2f, -1.64f, 8.0f, 1.0f });
	m_HeroAbilityRender[3]->WSCALE({ 0.52f, 0.55f });

	if (false == b_Is_Battle)
	{
		m_HeroAbilityRender[0]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[0]);
		m_HeroAbilityRender[1]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[1]);
		m_HeroAbilityRender[2]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[2]);
		m_HeroAbilityRender[3]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[3]);
	}
	else
	{
		b_Can_Activate_Skill[0] = true;
		b_Can_Activate_Skill[1] = true;
		b_Can_Activate_Skill[2] = true;
		b_Can_Activate_Skill[3] = true;

		if (false == CheckSkillLaunch(*m_HeroActor[_HeroIndex], 0, 0, false))
		{
			m_HeroAbilityRender[0]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[0]);
			b_Can_Activate_Skill[0] = false;
		}

		if (false == CheckSkillLaunch(*m_HeroActor[_HeroIndex], 1, 0, false))
		{
			m_HeroAbilityRender[1]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[1]);
			b_Can_Activate_Skill[1] = false;
		}

		if (false == CheckSkillLaunch(*m_HeroActor[_HeroIndex], 2, 0, false))
		{
			m_HeroAbilityRender[2]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[2]);
			b_Can_Activate_Skill[2] = false;
		}

		if (false == CheckSkillLaunch(*m_HeroActor[_HeroIndex], 3, 0, false))
		{
			m_HeroAbilityRender[3]->Sprite(m_HeroUISprite[_HeroIndex].m_HeroAbilityBlack[3]);
			b_Can_Activate_Skill[3] = false;
		}
	}

}

void SGAMECOMBAT::PrintTarget()
{
	CVector tempPos;

	//monster size에 따라서 스프라이트가 바뀐다.
	//가장 작음 = 0, 2칸 = 1
	//나중에 전투정보를 받아오면 해당 적의 크기 정보도 받아와야함
	//히어로들은 그냥 0
	m_TargetRender[0]->Sprite(m_TargetSprite[0]);
	m_TargetRender[1]->Sprite(m_TargetSprite[0]);
	m_TargetRender[2]->Sprite(m_TargetSprite[0]);
	m_TargetRender[3]->Sprite(m_TargetSprite[0]);

	for (int i = 0; i < 4; i++)
	{
		m_TargetRender[i]->WSCALE({ 1.2f, 1.4f });
		tempPos = LogicHeader::MonsterTargetPos[i];
		tempPos.y -= 1.55f;
		tempPos.z = LogicHeader::BackBottomZpos - 1.0f - (i * 0.1f);
		m_TargetRender[i]->WPOS(tempPos);
	}

	for (int i = 0; i < (int)m_CurCombat->second->m_MonsterData.size(); i++)
	{
		if (m_CurCombat->second->m_MonsterData[i]->m_MonsterData.m_DisplaySize > 1)
		{
			m_TargetRender[i]->Sprite(m_TargetSprite[1]);
			m_TargetRender[i]->WSCALE({ 1.4f, 1.0f, 7.0f, 1.0f });
		}
	}
}


void SGAMECOMBAT::ChangeSelectUI(const CVector& _Pos, int _UISpriteIndex/* = 0*/)
{
	CVector temp = _Pos;
	if (temp.x > 0)
	{
		temp.x += 0.05f;
	}
	else
	{
		temp.x -= 0.05f;
	}
	temp.y -= 1.55f;
	temp.z = LogicHeader::BackBottomZpos - 1.0f;
	m_SelectRender->Trans()->WPOS(temp);
	m_SelectRender->Sprite(m_SelectSprite[_UISpriteIndex]);

	m_SelectUISize = m_SelectRender->Trans()->WSCALE();
}

void SGAMECOMBAT::CreateCombat(const GameMiniMap::SKEY& _Key, std::vector<SPtr<GameActor>>& _Actor)
{
	SPtr<SGAMECOMBAT> NewCombat = new SGAMECOMBAT();
	SPtr<SMONSTERCOMBAT> tempCombat;

	for (int i = 0; i < (int)_Actor.size(); ++i)
	{
		NewCombat->m_MonsterActor.push_back(_Actor[i]);
		tempCombat = _Actor[i]->GetCom<SMONSTERCOMBAT>();
		tempCombat->m_CombatPosition = i;
		NewCombat->m_MonsterData.push_back(tempCombat);
	}

	if (NewCombat->m_MonsterData.size() > 4)
	{
		ASSERTMSG;
	}

	m_AllCombat.insert(std::map<__int64, SPtr<SGAMECOMBAT>>::value_type(_Key.TileKey, NewCombat));
}

bool SGAMECOMBAT::CheckCombat(const GameMiniMap::SKEY& _Key)
{
	m_CurCombat = m_AllCombat.find(_Key.TileKey);

	if (m_AllCombat.end() != m_CurCombat)
	{
		return true;
	}

	return false;
}

//가지고 있는 combat 정보에서 monster는 그냥 새로 데이터를 넣어주고(받아온 액터에 새걸로 덮어쓰기, 만들어진 액터가 바뀌는건 아니다.)
//몬스터 액터가 애니메이션 정보를 가지고 있으므로 그대로 활용하고 MonsterData만 새로 덮어쓴다. (몬스터는 늘 쌩쌩하니까)
//근데 어차피 돌려놓을 건 hp정보 하나 뿐이지 않나? 모두 새로 갈아치울 필요가 없네?
//그리고 몬스터 정보도 액터가 들고 있으니까 액터에서 역으로 가져오던가 아니면 인덱스만 기억해 두고 가져와도 되는데?
void SGAMECOMBAT::StartBattle(SPtr<GameCam> _Cam)
{
	m_SelectAbilitypriteRender->On();
	std::vector<GameActor*> tempActor = m_CurCombat->second->m_MonsterActor;
	SPtr<SMONSTERCOMBAT> MonsterCombatPtr;
	m_MainCam = _Cam;
	CVector tempPos;
	//전투 정보 초기화 단계 몬스터와 히어로 정보를 가져옴
	for (auto& MonsterActor : tempActor)
	{
		MonsterCombatPtr = MonsterActor->GetCom<SMONSTERCOMBAT>();
		tempPos = m_MainCam->Trans()->WPOS();
		tempPos += LogicHeader::MonsterCombatPos[MonsterCombatPtr->m_CombatPosition];
		b_Is_Hero_SKill_Target[MonsterCombatPtr->m_CombatPosition] = true;
		tempPos.z -= MonsterCombatPtr->m_CombatPosition * 0.1f;
		MonsterActor->Trans()->WPOS(tempPos);
	}

	tempActor = m_CurCombat->second->m_HeroActor;
	SPtr<SHEROCOMBAT> HeroCombatPtr;
	for (auto& HeroActor : m_HeroActor)
	{
		HeroCombatPtr = HeroActor->GetCom<SHEROCOMBAT>();
		tempPos = m_MainCam->Trans()->WPOS();
		tempPos += LogicHeader::PlayerCombatPos[HeroCombatPtr->m_CombatPosition];
		b_Is_Monster_SKill_Target[HeroCombatPtr->m_CombatPosition] = true;
		tempPos.z -= HeroCombatPtr->m_CombatPosition * 0.1f;
		HeroActor->Trans()->WPOS(tempPos);
		m_CurCombat->second->m_HeroData.push_back(HeroCombatPtr);
	}

	//스피드 순서에 따라 정렬
	for (auto& MonsterData : m_CurCombat->second->m_MonsterData)
	{
		m_CurCombat->second->m_SpeedArrange.push_back(std::make_pair(MonsterData->m_MonsterData.m_Status.m_Speed, MonsterData->Actor()));
	}

	for (auto& HeroData : m_CurCombat->second->m_HeroData)
	{
		m_CurCombat->second->m_SpeedArrange.push_back(std::make_pair(HeroData->m_HeroData->m_Status.m_Speed, HeroData->Actor()));
	}

	m_CurCombat->second->m_SpeedArrange.sort(compare);

	for (auto& pair : m_CurCombat->second->m_SpeedArrange)
	{
		m_CurCombat->second->m_SpeedStack.push(pair.second);
	}
}

static bool compare(std::pair<float, GameActor*> _left, std::pair<float, GameActor*> _right)
{
	return _left.first < _right.first;
}

bool SGAMECOMBAT::CheckSkillLaunch(GameActor& _CurActor, int _SkillNum, int _SkillLevel /*= 0*/, bool _ChangeCurSkill /*= true*/)
{
	if (true == _CurActor.HasCom<SHEROCOMBAT>())
	{
		SHEROCOMBAT* CurHeroCombat = _CurActor.GetCom<SHEROCOMBAT>();
		HeroData* CurHeroData = CurHeroCombat->m_HeroData;
		int CurPos = CurHeroCombat->m_CombatPosition;

		ActorList::PlayerEnum PlayerEnum = CurHeroCombat->m_HeroEnum;

		HeroData::HeroSkill* tempSkill = nullptr;

		switch (PlayerEnum)
		{
		case ActorList::PlayerEnum::crusader:
			tempSkill = CurHeroData->m_Skill.find(ActorList::crusader_SkillName[_SkillNum])->second[_SkillLevel];
			break;
		case ActorList::PlayerEnum::highwayMan:
			tempSkill = CurHeroData->m_Skill.find(ActorList::highwayman_SkillName[_SkillNum])->second[_SkillLevel];
			break;
		case ActorList::PlayerEnum::vestal:
			tempSkill = CurHeroData->m_Skill.find(ActorList::vestal_SkillName[_SkillNum])->second[_SkillLevel];
			break;
		case ActorList::PlayerEnum::plague_doctor:
			tempSkill = CurHeroData->m_Skill.find(ActorList::plague_doctor_SkillName[_SkillNum])->second[_SkillLevel];
			break;
		case ActorList::PlayerEnum::Max:
			break;
		default:
			break;
		}

		if (nullptr == tempSkill)
		{
			return false;
		}

		if (true == _ChangeCurSkill)
		{
			m_CurSkill = tempSkill;
		}

		return tempSkill->m_LaunchPos[CurPos];
	}

	if (false == _CurActor.HasCom<SMONSTERCOMBAT>())
	{
		ASSERTMSG;
	}

	{
		SMONSTERCOMBAT* CurMonsterCombat = _CurActor.GetCom<SMONSTERCOMBAT>();
		MonsterData* CurMonsterData = &CurMonsterCombat->m_MonsterData;
		int CurPos = CurMonsterCombat->m_CombatPosition;
		CombatSkill* tempSkill = nullptr;

		switch (_SkillNum)
		{
		case 0:
			tempSkill = &CurMonsterData->m_Skill[0];
			break;
		case 1:
			tempSkill = &CurMonsterData->m_Skill[1];
			break;
		case 2:
			tempSkill = &CurMonsterData->m_Skill[2];
			break;
		case 3:
			tempSkill = &CurMonsterData->m_Skill[3];
			break;
		default:
			break;
		}

		if (nullptr == tempSkill)
		{
			return false;
		}

		if (true == _ChangeCurSkill)
		{
			m_CurMonsterSkill = tempSkill;
		}

		return tempSkill->m_LaunchPos[_SkillNum];
	}

}

void SGAMECOMBAT::CheckSkillTarget(std::vector<bool>* _TargetPos)
{
	if (nullptr == m_CurSkill)
	{
		ASSERTMSG;
	}

	_TargetPos->clear();

	for (int i = 0; i < 4; i++)
	{
		if (true == m_CurSkill->m_TargetPos[i] && true == b_Is_Hero_SKill_Target[i])
		{
			_TargetPos->push_back(true);
			m_TargetRender[i]->On();
		}
		else
		{
			_TargetPos->push_back(false);
			m_TargetRender[i]->Off();
		}
	}

	if (_TargetPos->size() != 4)
	{

	}

	PrintTarget();
}

std::vector<int> SGAMECOMBAT::CheckMonsterSkillTarget()
{
	m_MonsterTarget.clear();
	m_MonsterTarget.reserve(4);

	if (nullptr == m_CurSkill)
	{
		ASSERTMSG;

	}

	for (int i = 0; i < 4; i++)
	{
		if (true == m_CurMonsterSkill->m_TargetPos[i] && true == b_Is_Monster_SKill_Target[i])
		{
			m_MonsterTarget.push_back(i);
		}
	}

	if (m_MonsterTarget.size() == 0)
	{
		ASSERTMSG;
	}

	return m_MonsterTarget;
}

void SGAMECOMBAT::CalcDamge(int _TargetPos)
{
	if (true == m_CurSkill->m_AOE)
	{

	}

	m_CurCombat->second->m_MonsterData[_TargetPos]->m_MonsterData.m_Status.m_Hp -= m_CurHeroCombat->m_HeroData->m_Status.m_MaxDmg;


	if (m_CurCombat->second->m_MonsterData[_TargetPos]->m_MonsterData.m_Status.m_Hp < 0)
	{
		m_CurCombat->second->m_MonsterActor[_TargetPos]->Off();
		m_MonsterActorIter = m_CurCombat->second->m_MonsterActor.begin();
		m_MonsterDataIterator = m_CurCombat->second->m_MonsterData.begin();

		for (int i = 0; i < _TargetPos; i++)
		{
			++m_MonsterActorIter;
			++m_MonsterDataIterator;
		}
		b_Is_Hero_SKill_Target[m_CurCombat->second->m_MonsterData.size() - 1] = false;
		m_CurCombat->second->m_MonsterActor.erase(m_MonsterActorIter);
		m_CurCombat->second->m_MonsterData.erase(m_MonsterDataIterator);
		RearragneMonster();
	}
}

void SGAMECOMBAT::CalcHeroDamge(CombatSkill* _CurSkill, int _TargetPos)
{
	if (true == m_CurSkill->m_AOE)
	{

	}

	m_CurCombat->second->m_HeroData[_TargetPos]->m_HeroData->m_Status.m_Hp -= _CurSkill->m_DmgMax;
}


void SGAMECOMBAT::Update()
{
	if (1 > m_SelectAbilitypriteRender->Color().w)
	{
		CVector tempVec = m_SelectAbilitypriteRender->Color();
		tempVec.w += GameTime::DeltaTime() * 4;
		m_SelectAbilitypriteRender->Color(tempVec);
	}

	if (m_SelectUITimer > 0)
	{
		m_SelectUITimer -= GameTime::DeltaTime();
	}

	if (m_SelectUITimer > 0.2f && m_SelectUISize.x < m_MaxSelectUIWidth)
	{
		m_SelectUISize.x += GameTime::DeltaTime() / 2;
		if (m_SelectUISize.x >= m_MaxSelectUIWidth)
		{
			m_SelectUISize.x -= GameTime::DeltaTime() / 2;
		}
	}
	else if (m_SelectUITimer > 0 && m_SelectUISize.x > m_MinSelectUIWidth)
	{
		m_SelectUISize.x -= GameTime::DeltaTime() * 2.0f;
	}
	else
	{
		m_SelectUITimer = 1.25f;
	}

	m_SelectRender->Trans()->WSCALE(m_SelectUISize);

	PrintFont(m_HeroIndex);
	PrintHeroUI(m_HeroIndex);

}

void SGAMECOMBAT::CloseTarget()
{
	for (int i = 0; i < 4; i++)
	{
		m_TargetRender[i]->Off();
	}
}

void SGAMECOMBAT::RearragneMonster()
{
	std::vector<GameActor*> tempActor = m_CurCombat->second->m_MonsterActor;
	SPtr<SMONSTERCOMBAT> MonsterCombatPtr;
	CVector tempPos;

	for (int i = 0; i < tempActor.size(); ++i)
	{
		MonsterCombatPtr = tempActor[i]->GetCom<SMONSTERCOMBAT>();
		tempPos = m_MainCam->Trans()->WPOS();
		tempPos += LogicHeader::MonsterCombatPos[i];
		b_Is_Hero_SKill_Target[i] = true;
		tempPos.z -= i * 0.1f;
		tempActor[i]->Trans()->WPOS(tempPos);
	}
}


void SGAMECOMBAT::PrintFont(int _HeroIndex /*= 0*/)
{
	m_CurHeroCombat = m_HeroActor[_HeroIndex]->GetCom<SHEROCOMBAT>();

	wchar_t buff[50];
	swprintf_s(buff, L"%s", m_CurHeroCombat->m_HeroData->m_Name.m_Str.c_str());
	GameDebugEn::Draw2DFont(buff, 15.0f, { 270.0f, 510.0f }, { 0.63f, 0.57f, 0.38f, 1.0f });
	swprintf_s(buff, L"%s", ActorList::PlayerListKor[_HeroIndex].m_Str.c_str());
	GameDebugEn::Draw2DFont(buff, 13.0f, { 280.0f, 530.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	int m_MaxHp = (int)m_CurHeroCombat->m_HeroData->m_Armor[m_CurHeroCombat->m_HeroArmorIndex]->m_Hp;
	swprintf_s(buff, L"%d/%d", (int)m_CurHeroCombat->m_HeroData->m_Status.m_Hp, m_MaxHp);
	GameDebugEn::Draw2DFont(buff, 13.0f, { 245.0f, 575.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	swprintf_s(buff, L"%d/%d", (int)m_CurHeroCombat->m_HeroData->m_Status.m_Stress, 200);
	GameDebugEn::Draw2DFont(buff, 13.0f, { 245.0f, 595.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"명중            %d", (int)m_CurHeroCombat->m_HeroData->m_Status.m_BonusHit);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 615.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"치명타        %.1f%%", m_CurHeroCombat->m_HeroData->m_Status.m_Crit);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 630.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"피해            %d-%d", (int)m_CurHeroCombat->m_HeroData->m_Status.m_MinDmg, (int)m_CurHeroCombat->m_HeroData->m_Status.m_MaxDmg);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 645.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"회피            %d", 0);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 660.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"방어            %d%%", (int)m_CurHeroCombat->m_HeroData->m_Status.m_Prot);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 675.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"속도            %d", (int)m_CurHeroCombat->m_HeroData->m_Status.m_Speed);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 205.0f, 690.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	swprintf_s(buff, L"%d", (int)m_CurHeroCombat->m_HeroWeaponIndex + 1);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 380.0f, 578.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	swprintf_s(buff, L"%d", (int)m_CurHeroCombat->m_HeroArmorIndex + 1);
	GameDebugEn::Draw2DFont(buff, 12.0f, { 435.0f, 578.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
}

/////HeroData

void SHEROCOMBAT::LoadHeroStatus()
{
	m_HeroData->m_Status.m_Hp = m_HeroData->m_Armor[m_HeroArmorIndex]->m_Hp;
	m_HeroData->m_Status.m_Def = m_HeroData->m_Armor[m_HeroArmorIndex]->m_Def;
	m_HeroData->m_Status.m_Prot = m_HeroData->m_Armor[m_HeroArmorIndex]->m_Prot;
	m_HeroData->m_Status.m_Speed = m_HeroData->m_Weapon[m_HeroWeaponIndex]->m_Speed;
	m_HeroData->m_Status.m_Crit = m_HeroData->m_Weapon[m_HeroWeaponIndex]->m_Crit;
	m_HeroData->m_Status.m_MinDmg = m_HeroData->m_Weapon[m_HeroWeaponIndex]->m_MinDmg;
	m_HeroData->m_Status.m_MaxDmg = m_HeroData->m_Weapon[m_HeroWeaponIndex]->m_MaxDmg;
}