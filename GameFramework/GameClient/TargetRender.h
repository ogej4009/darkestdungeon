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
	//플레이어 액터에 넣어두고 스킬들 애니메이션을 4개 가지고 있다가 타겟 대상의 위치에 필요에 따라 랜더링

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

