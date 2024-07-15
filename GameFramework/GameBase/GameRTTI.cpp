#include "GameRTTI.h"


GameRTTI::GameRTTI()
{
}

GameRTTI::~GameRTTI()
{
}

void GameRTTI::TypeSetting()
{
	m_TypeInfo = &typeid(*this);
}