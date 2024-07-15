#include "GameComponent.h"

GameComponent::GameComponent() : m_pActor(nullptr)
{
}


GameComponent::~GameComponent()
{
}

void GameComponent::Init()
{

}

//부모의 Trans정보를 가져오게 해주기
GameTransform* GameComponent::Trans()
{
	return m_pActor->Trans();
}