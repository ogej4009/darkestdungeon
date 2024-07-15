#include "LogicHeader.h"

const float LogicHeader::MinimapZpos = 10.0f;
const float LogicHeader::PlayerZpos = 10.6f; //0.1씩 다음 플레이어가 감소한 z값을 가진다.(시작 11.0f)
const float LogicHeader::MonsterZpos = 10.4f; // 0.1씩 다음 몬스터가 감소한 z값을 가진다.(시작 10.8f)
const float LogicHeader::BackBottomZpos = 9.0f;
const CVector LogicHeader::WindowSize = { 1280.0f ,  720.0f };
const CVector LogicHeader::ScreenSize = { 12.8f, 7.2f };
const CVector LogicHeader::UpScreenSize = { 12.8f, 4.85f };
const CVector LogicHeader::CorridorSize = { 4.25f, 4.0f };
const CVector LogicHeader::CorridorMidSize = { 4.25f, 6.0f };
const CVector LogicHeader::DoorSize = { 6.2f, 3.4f };
const CVector LogicHeader::CorridorPos = { -7.0f, 0.8f, 20.0f, 1.0f };
const CVector LogicHeader::UpScreenPos = { 0.0f, 1.2f, 20.0f, 1.0f };
const CVector LogicHeader::PlayerStartPos = { -1.2f, 0.9f, 11.0f, 0.0f };
const CVector LogicHeader::PlayerCombatPos[4] = { { -1.2f, 0.9f, 11.0f, 1.0f }, { -2.3f, 0.9f, 11.0f, 1.0f }, { -3.4f, 0.9f, 11.0, 1.0f }, { -4.5f, 0.9f, 11.0, 1.0f } };
const CVector LogicHeader::MonsterStartPos = { 1.3f, 0.9f, 10.8f, 0.0f };
const CVector LogicHeader::MonsterCombatPos[4] = { { 1.3f, 0.9f, 10.8f, 1.0f }, { 2.5f, 0.9f, 10.8f, 1.0f }, { 3.5f, 0.9f, 10.8f, 1.0f }, { 4.6f, 0.9f, 10.8f, 1.0f } };
const CVector LogicHeader::MonsterTargetPos[4] = { { 1.5f, 0.9f, 10.8f, 1.0f }, { 2.7f, 0.9f, 10.8f, 1.0f }, { 3.7f, 0.9f, 10.8f, 1.0f }, { 4.7f, 0.9f, 10.8f, 1.0f } };
const CVector LogicHeader::DefaultDarkImgSize = { 6.2f, 4.07f, 1.0f };
const CVector LogicHeader::DefaultDarkMonsterImgSize[2] = { { -9.27f, 4.26f, 1.0f }, {-6.91f, 4.4f, 1.0f} };
const float LogicHeader::DefaultDarkMonsterYPos[2] = { 0, 0.0f };
const CVector LogicHeader::SelectUIPlayerXOffset = { -0.1f, 0, 0, 0 };
const CVector LogicHeader::SelectUIMonsterXOffset = { 0.4f, 0, 0, 0 };
const GameRect LogicHeader::MinimapRange = { {0.05f, -1.28f, 10.0f, 1.0f}, {4.48f, -3.46f, 10.0f, 1.0f} };
const GameRect LogicHeader::FocusCamRange = { {4.5f, -1.2f, 10.0f, 1.0f}, {4.8f, -1.7f, 10.0f, 1.0f} };
const GameRect LogicHeader::TargetRange[4] = { {{6.2f, 0.4f},{6.8f, -0.2f}},{{7.5f, 0.4f},{8.0f, -0.2f}},{{8.5f, 0.4f},{9.0f, -0.2f}},{{9.7f, 0.4f},{10.2f, -0.2f}} };

const GameString ActorList::crusader_SkillName[7] = { L"smite" , L"zealous_accusation" , L"stunning_blow" , L"bulwark_of_faith"  , L"battle_heal"  , L"holy_lance"  , L"inspiring_cry" };
const GameString ActorList::highwayman_SkillName[7] = { L"wicked_slice" ,L"pistol_shot" , L"point_blank_shot" , L"grape_shot_blast" , L"take_aim" , L"duelist_advance" , L"opened_vein" };
const GameString ActorList::vestal_SkillName[7] = { L"mace_bash" ,L"judgement" , L"dazzling_light" , L"divine_grace" ,  L"gods_comfort" , L"gods_illumination" , L"gods_hand" };
const GameString ActorList::plague_doctor_SkillName[7] = { L"noxious_blast" ,L"plague_grenade" , L"blinding_gas" , L"incision" , L"battlefield_medicine" , L"emboldening_vapours" , L"disorienting_blast" };

GameString ActorList::TargetSkillName[7] = { L"Skill1Target",L"Skill2Target" ,L"Skill3Target" ,L"Skill4Target" ,L"Skill5Target" ,L"Skill6Target" ,L"Skill7Target" };
GameString ActorList::PlayerList[4] = { L"crusader", L"highwayman", L"vestal", L"plague_doctor" };
GameString ActorList::PlayerListKor[4] = { L"  성전사", L"노상강도", L"    성녀", L"역병의사" };
GameString ActorList::CryptMonsterList[] = { L"skeleton_arbalist",L"skeleton_bearer", L"skeleton_captain",L"skeleton_common",L"skeleton_courtier",L"skeleton_defender",L"skeleton_militia",L"skeleton_spear" };
GameString ActorList::mapList[6] = { L"cove",L"crypts", L"darkestdungeon", L"warrens", L"weald", L"common" };
GameString ActorList::PlayerName[4] = { L"레이널드",L"디스마스", L"파이드케어", L"래트게이버" };
GameString ActorList::MonsterName[4] = { L"도적출혈기술자",L"도적총포수", L"도적살인자", L"평범한해골" };