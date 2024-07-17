#include "pch.h"
#include "ToolProgress.h"
#include <GameWin.h>
#include <GameDevice.h>
#include <GameScene.h>
#include <GameInput.h>
#include <GameTime.h>
#include <GameDic.h>
#include <GameTexture.h>
#include <GameSprite.h>
#include "ToolMaker.h"


ToolProgress::ToolProgress()
{
}


ToolProgress::~ToolProgress()
{
}

void ToolProgress::Init()
{
	GameScene::CreateScene<ToolMaker>("MAP");
	GameScene::CreateScene("OBJECT");
	GameScene::ChangeScene("MAP");
}


void ToolProgress::Progress()
{
	//map<int, List<Room*>>

	//int MaxRoomCount = 5;
	//list<Room*> m_RoomOpenList;
	//m_RoomOpenList.Push(RandomMap());

	//map<__int64, Room*> m_Map;

	//while (true)
	//{
	//	if (0 == MaxRoomCount)
	//	{

	//	}

	//	if (0 != m_RoomList.size() && 0 != MaxRoomCount)
	//	{
	//		Room* CreateRoom = m_RoomList.front();
	//		m_RoomList.popfront();


	//		// 2
	//		int Count = random(4);
	//		MaxRoomCount -= Count;

	//		DIR ArrDir[4] = {DIR_LEFT, DIR_RIGHT, DIR_TOP, DIR_BOT };

	//		for (size_t i = 0; i < 10; i++)
	//		{
	//			// ¼ÅÇÃ
	//		}

	//		for (size_t i = 0; i < DIR_MAX - Count; i++)
	//		{
	//			ArrDir[i] = DIR_MAX;
	//		}

	//		// ArrDir[4] = { DIR_LEFT, DIR_MAX, DIR_TOP, DIR_MAX };

	//		for (size_t i = 0; i < DIR_MAX; i++)
	//		{
	//			if (ArrDir[i] != DIR_MAX)
	//			{

	//			}
	//		}

	//		m_Map.insert(CreateRoom);
	//	}
	//}

	GameTime::Update();
	GameInput::Update();
	GameScene::Progress();
}
