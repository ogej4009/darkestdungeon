#include "Hero.h"
#include <GameImage.h>
#include <GameAni.h>
#include <GameInput.h>
#include <LifeCycleLogic.h>
#include <GameDebugEn.h>
#include <LogicHeader.h>



Hero::Hero() : m_Speed(2.0f)
{
}

Hero::~Hero()
{
}

void Hero::GEnterEvent(GameCol* _This, GameCol* _Other)
{

}

void Hero::EnterEvent(GameCol* _This, GameCol* _Other)
{

}

void Hero::FootPrint(GameLogic* _Logic)
{
}

void Hero::Init()
{
	SPtr<GameImage> RENDER = AddCom<GameImage>();// 플레이어 그림 띄워주는 랜더러
	ANICOM = AddCom<GameAni>();// 플레이어 그림 띄워주는 랜더러
	ANICOM->CreateAni(L"Walk", L"ShovelKnight.png", 2, 7, 0.5f, true);
	ANICOM->CreateAni(L"Att", L"ShovelKnight.png", 8, 14, 0.1f, false);

	//ANICOM->AddStartEvent(L"Att", 10, this, &Hero::FootPrint);
	StateChange(L"Walk");
	RENDER->Sprite(L"ShovelKnight.png");
}

void Hero::StateChange(const GameString& _StateName)
{
	m_StateName = _StateName;
	ANICOM->ChangeAni(_StateName);
}

bool Hero::IsState(const GameString& _StateName)
{
	return m_StateName == _StateName;
}

void Hero::Walk()
{
	// size_t All = COL->m_OtherCol.size();
/*
	if (true == HGAMEINPUT::Press(L"LMOVE"))
	{
		Trans()->ScaleXMinus();
		Trans()->LMOVETOTIMESPEED(HVEC4::LEFT, m_Speed);
	}

	if (true == HGAMEINPUT::Press(L"RMOVE"))
	{
		Trans()->ScaleXPlus();
		Trans()->LMOVETOTIMESPEED(HVEC4::RIGHT, m_Speed);
	}

	if (true == HGAMEINPUT::Press(L"UMOVE"))
	{
		Trans()->LMOVETOTIMESPEED(HVEC4::UP, m_Speed);
	}
	if (true == HGAMEINPUT::Press(L"DMOVE"))
	{
		Trans()->LMOVETOTIMESPEED(HVEC4::DOWN, m_Speed);
	}

	if (true == HGAMEINPUT::Press(L"PROT"))
	{
		Trans()->LROTZ(360.0F* HGAMETIME::DeltaTime());
	}


	if (true == HGAMEINPUT::Press(L"MROT"))
	{
		Trans()->LROTZ(-360.0F * HGAMETIME::DeltaTime());
	}


	if (true == HGAMEINPUT::Press(L"ATT"))
	{
		StateChange(L"Att");

		HGAMESOUND::SoundStop(L"BGM");
	}*/
}


void Hero::Att() {

	if (true == ANICOM->IsCurAni(L"Att")
		&& true == ANICOM->IsCurAniEnd())
	{
		StateChange("Walk");
	}

}

void Hero::Update()
{
	// H3DDEBUG::Draw2DRect({ 100, 100 }, { 200, 200 });

	if (IsState("Walk"))
	{
		Walk();
	}
	else if (IsState("Att"))
	{
		Att();
	}
}

void Hero::DebugRender() {
}