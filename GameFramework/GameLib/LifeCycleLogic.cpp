#include "LifeCycleLogic.h"
#include <GameTime.h>

LifeCycleLogic::LifeCycleLogic() : m_DeathTime(0.0f)
{

}

LifeCycleLogic::~LifeCycleLogic()
{

}

void LifeCycleLogic::Init(float _DeathTime)
{
	m_DeathTime = _DeathTime;
}

void LifeCycleLogic::Update()
{
	m_DeathTime -= GameTime::DeltaTime();

	if (0 >= m_DeathTime)
	{
		Actor()->Death();
	}
}