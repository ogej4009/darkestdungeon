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

//�θ��� Trans������ �������� ���ֱ�
GameTransform* GameComponent::Trans()
{
	return m_pActor->Trans();
}