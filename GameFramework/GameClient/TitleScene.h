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

//GAMECOMBAT�� �޾Ƴ��� �� ����� ���ִ� ���� �Ŵ���
//�ش� ������ FSM�� ó�� ����. �ٵ� FSM�� ��� ó���ؾ� ����?->������ �ѹ� �������.
//������ ���͵� FSM�� �������� �κ��� ������ �������� FSMó�� ������Ʈ�� ����� �ʹ�.
//UI ��ư �Է� -> FSMó�� ������Ʈ�� �����Ŵ����� ī�޶� ����
//FSMó�� ������Ʈ�� ���� ����
//�÷��̾� Ŭ������ ���� ������ �ϳ�? ���Ͱ� ������ �� ��� �ִµ�?
//m_PlayerPTR->GetCom<SGAMECOMBAT>()->Attack(m_EnemyPTR->GetCom<SGAMECOMBAT>());

//wchar_t buff[100];
//swprintf_s(buff, L"Skel's Health : %3f", m_EnemyPTR->GetCom<SGAMECOMBAT>()->MonsterHealth());
//GameDebugEn::Draw2DFont(buff, 20.0f, { 0.0f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

//
//#pragma once
//#include <SGAMESCENE.h>
//
////���� ���Ͱ� �ʿ���� ������Ʈ���� ������� ����� SCENCOM
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
