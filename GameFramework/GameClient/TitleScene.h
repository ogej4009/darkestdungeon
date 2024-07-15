#pragma once
#include <GameScene.h>



class GameGrid;
class GameActor;
class TitleScene : public GameScene::SceneComponent
{
public:
	SPtr<GameActor> CamWin;
	SPtr<GameGrid> GridRender;

public:
	void Loading() override;
	void Update() override;
	void DebugRender() override;

public:
	TitleScene();
	~TitleScene();
};

//GAMECOMBAT을 받아놓고 턴 계산을 해주는 전투 매니저
//해당 액터의 FSM도 처리 가능. 근데 FSM은 어떻게 처리해야 하지?->저번에 한번 보여줬다.
//어차피 액터들 FSM은 공통적인 부분이 많은데 공통적인 FSM처리 컴포넌트를 만들고 싶다.
//UI 버튼 입력 -> FSM처리 컴포넌트와 전투매니저와 카메라에 전달
//FSM처리 컴포넌트는 동작 변경
//플레이어 클래스를 따로 만들어야 하나? 액터가 정보를 다 들고 있는데?
//m_PlayerPTR->GetCom<SGAMECOMBAT>()->Attack(m_EnemyPTR->GetCom<SGAMECOMBAT>());

//wchar_t buff[100];
//swprintf_s(buff, L"Skel's Health : %3f", m_EnemyPTR->GetCom<SGAMECOMBAT>()->MonsterHealth());
//GameDebugEn::Draw2DFont(buff, 20.0f, { 0.0f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

//
//#pragma once
//#include <SGAMESCENE.h>
//
////따로 엑터가 필요없는 컴포넌트들을 대상으로 만드는 SCENCOM
//class STITLECOM : public GameScene::SceneComponent
//{
//private:
//	float m_FPS;
//	float m_FadeTimer;
//	GameString m_FPSstr;
//
//	GameActor* m_EnemyPTR;
//	SPtr<GameActor> m_PlayerPTR;
//
//public:
//	STITLECOM();
//	~STITLECOM();
//
//public:
//	void Loading() override;
//	void Update() override;
//	void DebugRender() override;
//
//
//};
//
