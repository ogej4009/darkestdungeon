#pragma once
#include <vector>
#include <GameLogic.h>
#include <LogicHeader.h>

class GameCam;
class GameAni;
class GameSprite;
class GameImage;
class TargetRender : public GameLogic
{
	//�÷��̾� ���Ϳ� �־�ΰ� ��ų�� �ִϸ��̼��� 4�� ������ �ִٰ� Ÿ�� ����� ��ġ�� �ʿ信 ���� ������

private:
	SPtr<GameActor> m_TargetActor[4];
	SPtr<GameImage> m_ImgRender[4];
	SPtr<GameAni> m_Ani[4];
	std::vector<SPtr<GameSprite>> m_AllSprite;
	static SPtr<GameSprite> m_TargetBlood;
	static std::vector<CVector> m_TargetBloodUVData;
	static int m_TargetBloodUVDataSize;
	float m_TargetRenderTimer;

public:
	TargetRender();
	~TargetRender();

public:
	void Init();
	void HeroTargetInit(const GameString& _ActorName);
	void Update() override;

public:
	void TargetRenderOn(int _SKillNum, SPtr<GameCam> _CAM, std::vector<bool> _Target);
};

