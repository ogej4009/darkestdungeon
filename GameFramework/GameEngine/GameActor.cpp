#include "GameActor.h"
#include "GameComponent.h"


GameActor::GameActor()
{
}

GameActor::~GameActor()
{
	for (auto& Com : m_AllCom)
	{
		Com->Death();
	}

}

void GameActor::ChangeScene(GameString& _Name)
{

}


void GameActor::UpdatePrev()
{
	for (auto& Actor : m_AllCom)
	{
		Actor->UpdatePrev();
	}
}
void GameActor::Update()
{
	for (auto& Actor : m_AllCom)
	{
		Actor->Update();
	}
}
void GameActor::UpdateNext()
{
	for (auto& Actor : m_AllCom)
	{
		Actor->UpdateNext();
	}
}

void GameActor::DebugRender()
{
	for (auto& Com : m_AllCom)
	{
		Com->DebugRender();
	}
}

void GameActor::DeleteCom()
{
	
	for (auto& Com : m_AllCom)
	{
		Com->Death();
	}

	m_StartIter = m_AllCom.begin();
	m_EndIter = m_AllCom.end();

	for (; m_StartIter != m_EndIter;)
	{
		//액터가 죽었으면 지워주렴
		if (false == (*m_StartIter)->IsDeath())
		{
			++m_StartIter;
			continue;
		}

		m_StartIter = m_AllCom.erase(m_StartIter);
	}
}