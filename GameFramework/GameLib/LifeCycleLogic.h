#pragma once
#include <SLOGIC.h>

class LifeCycleLogic : public GameLogic
{
private:
	float m_DeathTime;

public :
	LifeCycleLogic();
	~LifeCycleLogic();

public:
	void Init(float _DeathTime);
	void Update() override;
};

// SPINE