#include "GameMiniMap.h"
#include "GameCam.h"
#include <GameMath.h>
#include "GameImage.h"
#include "GameActor.h"

SPtr<GameSprite> GameMiniMap::m_RoomSprite[8];
SPtr<GameSprite> GameMiniMap::m_RoadSprite[3];
GameRect GameMiniMap::MinimapRange = { {0.05f, -1.28f, 10.0f, 1.0f}, {4.48f, -3.46f, 10.0f, 1.0f} };

GameMiniMap::SKEY GameMiniMap::ArrCheck[4] = {
	GameMiniMap::SKEY(-1, 0),
	GameMiniMap::SKEY(0, 1),
	GameMiniMap::SKEY(1, 0),
	GameMiniMap::SKEY(0, -1),
};

GameMiniMap::RoadDIR GameMiniMap::ArrTILE4WAYDIR[4] = { GameMiniMap::RoadDIR::L, GameMiniMap::RoadDIR::R, GameMiniMap::RoadDIR::T, GameMiniMap::RoadDIR::B };

//다 한장짜리 이미지이고 따로 color변화가 따로 필요하지 않아서 공통으로 Color와 Index를 공유함
GameMiniMap::GameMiniMap() : m_Color(CVector::WHITE), m_CutData({ 0,0,1,1 })
{
	srand((unsigned int)time(nullptr));
}

GameMiniMap::~GameMiniMap()
{
	Clear();
}

void GameMiniMap::Render(SPtr<GameCam> _Cam)
{

	GameTransform::Update();
	//GameTransform::CalTransData(_Cam);
	CalTransData(_Cam);


	for (auto& RD : m_RenderList)
	{
		if (false == RD->IsUpdate())
		{
			continue;
		}

		RD->Render();
	}
}

void GameMiniMap::SpriteCheck()
{
}

void GameMiniMap::SpriteLoad()
{
	std::vector<GameString> RoadName;
	RoadName.push_back(L"hall_clear.png");
	RoadName.push_back(L"hall_dark.png");
	RoadName.push_back(L"hall_dim.png");

	std::vector<GameString> RoomName;
	RoomName.push_back(L"room_battle.png");
	RoomName.push_back(L"room_boss.png");
	RoomName.push_back(L"room_curio.png");
	RoomName.push_back(L"room_empty.png");
	RoomName.push_back(L"room_entrance.png");
	RoomName.push_back(L"room_treasure.png");
	RoomName.push_back(L"room_unknown.png");
	RoomName.push_back(L"moving_room.png");

	RoadBlockSprite(RoadName);
	RoomSprite(RoomName);

	m_TrayResolve = GameSprite::Find(L"indicator.png");
}

void GameMiniMap::RoadBlockSprite(const std::vector<GameString>& _Name)
{
	for (int i = 0; i < _Name.size(); ++i)
	{
		m_RoadSprite[i] = GameSprite::Find(_Name[i]);

		if (nullptr == m_RoadSprite[i])
		{
			ASSERTMSG;
		}
	}
}

void GameMiniMap::RoomSprite(const std::vector<GameString>& _Name)
{
	for (int i = 0; i < _Name.size(); ++i)
	{
		m_RoomSprite[i] = GameSprite::Find(_Name[i]);

		if (nullptr == m_RoomSprite[i])
		{
			ASSERTMSG;
		}
	}
}

bool GameMiniMap::CheckRange(const CTransData& _Trans)
{
	if (MinimapRange.LeftTop.x > _Trans.W.ArrVec[3].x || MinimapRange.RightBottom.x < _Trans.W.ArrVec[3].x)
	{
		return false;
	}

	if (MinimapRange.LeftTop.y < _Trans.W.ArrVec[3].y || MinimapRange.RightBottom.y > _Trans.W.ArrVec[3].y)
	{
		return false;
	}

	return true;
}

void GameMiniMap::CalTransData(SPtr<GameCam> _Cam)
{
	// 각 타일의 트랜스 데이터를 업데이트 할것이다.
	CVector TilePos;

	for (auto& road : m_AllRoadBlockVec)
	{
		road->m_TransData.W = Actor()->Trans()->CREFWW();

		TilePos.x = road->m_TransData.W.ArrVec[3].x + (float)(road->m_Key.X * road->m_TransData.W.ArrVec[0].x);
		TilePos.y = road->m_TransData.W.ArrVec[3].y + (float)(road->m_Key.Y * road->m_TransData.W.ArrVec[1].y);
		TilePos.z = road->m_TransData.W.ArrVec[3].z;
		//CVector 초기화가 0,0,0,0이라서 1을 채워넣지 않으면 기존 데이터가 변조된다.
		TilePos.w = 1;

		road->m_TransData.W.ArrVec[3] = TilePos;
		road->m_TransData.V = _Cam->MatView();
		road->m_TransData.P = _Cam->MatProj();
		road->m_TransData.WV = road->m_TransData.W * road->m_TransData.V;
		road->m_TransData.VP = _Cam->MatViewProj();
		road->m_TransData.WVP = road->m_TransData.W * road->m_TransData.V * road->m_TransData.P;

		if (false == CheckRange(road->m_TransData))
		{
			road->RD->Off();
		}
		else
		{
			road->RD->On();
		}

		road->m_TransData.AllTP();


	}

	{
		if (nullptr != m_MinimapIndicator)
		{
			m_MinimapIndicator->m_TransData.W = Actor()->Trans()->CREFWW();
			TilePos.x = m_MinimapIndicator->m_TransData.W.ArrVec[3].x + (float)(m_MinimapIndicator->m_Key.X * m_MinimapIndicator->m_TransData.W.ArrVec[0].x);
			TilePos.y = m_MinimapIndicator->m_TransData.W.ArrVec[3].y + (float)(m_MinimapIndicator->m_Key.Y * m_MinimapIndicator->m_TransData.W.ArrVec[1].y);
			TilePos.z = m_MinimapIndicator->m_TransData.W.ArrVec[3].z;
			//CVector 초기화가 0,0,0,0이라서 1을 채워넣지 않으면 기존 데이터가 변조된다.
			TilePos.w = 1;

			m_MinimapIndicator->m_TransData.W.ArrVec[3] = m_MinimapIndicatorPos = TilePos;
			m_MinimapIndicator->m_TransData.W.Arr2D[3][2] -= 0.3f;
			m_MinimapIndicator->m_TransData.W.Arr2D[0][0] = m_MinimapIndicator->m_TransData.W.Arr2D[1][1] = m_MinimapIndicator->m_TransData.W.Arr2D[2][2] = m_MinimapIndicator->m_TransData.W.Arr2D[2][2] * 2.0F;
			m_MinimapIndicator->m_TransData.V = _Cam->MatView();
			m_MinimapIndicator->m_TransData.P = _Cam->MatProj();
			m_MinimapIndicator->m_TransData.WV = m_MinimapIndicator->m_TransData.W * m_MinimapIndicator->m_TransData.V;
			m_MinimapIndicator->m_TransData.VP = _Cam->MatViewProj();
			m_MinimapIndicator->m_TransData.WVP = m_MinimapIndicator->m_TransData.W * m_MinimapIndicator->m_TransData.V * m_MinimapIndicator->m_TransData.P;

			if (false == CheckRange(m_MinimapIndicator->m_TransData))
			{
				m_MinimapIndicator->RD->Off();
			}
			else
			{
				m_MinimapIndicator->RD->On();
			}

			m_MinimapIndicator->m_TransData.AllTP();
		}
	}

	for (auto& room : m_AllRoomVec)
	{

		room->m_TransData.W = Actor()->Trans()->CREFWW();

		TilePos.x = room->m_TransData.W.ArrVec[3].x + (float)(room->m_Key.X * room->m_TransData.W.ArrVec[0].x);
		TilePos.y = room->m_TransData.W.ArrVec[3].y + (float)(room->m_Key.Y * room->m_TransData.W.ArrVec[1].y);
		TilePos.z = room->m_TransData.W.ArrVec[3].z - 0.1f;
		//CVector 초기화가 0,0,0,0이라서 1을 채워넣지 않으면 기존 데이터가 변조된다.
		TilePos.w = 1;
		room->m_TransData.W.ArrVec[3] = TilePos;
		room->m_TransData.W.Arr2D[0][0] = room->m_TransData.W.Arr2D[1][1] = room->m_TransData.W.Arr2D[2][2] = room->m_TransData.W.Arr2D[0][0] * 2.8f;
		room->m_TransData.V = _Cam->MatView();
		room->m_TransData.P = _Cam->MatProj();
		room->m_TransData.WV = room->m_TransData.W * room->m_TransData.V;
		room->m_TransData.VP = _Cam->MatViewProj();
		room->m_TransData.WVP = room->m_TransData.W * room->m_TransData.V * room->m_TransData.P;

		room->m_MoveTransData.W = Actor()->Trans()->CREFWW();

		TilePos.x = room->m_MoveTransData.W.ArrVec[3].x + (float)(room->m_Key.X * room->m_MoveTransData.W.ArrVec[0].x);
		TilePos.y = room->m_MoveTransData.W.ArrVec[3].y + (float)(room->m_Key.Y * room->m_MoveTransData.W.ArrVec[1].y);
		TilePos.z = room->m_MoveTransData.W.ArrVec[3].z - 0.2f;
		//CVector 초기화가 0,0,0,0이라서 1을 채워넣지 않으면 기존 데이터가 변조된다.
		TilePos.w = 1;
		room->m_MoveTransData.W.Arr2D[0][0] = room->m_MoveTransData.W.Arr2D[1][1] = room->m_MoveTransData.W.Arr2D[2][2] = room->m_MoveTransData.W.Arr2D[0][0] * 2.8f;
		room->m_MoveTransData.W.ArrVec[3] = TilePos;
		room->m_MoveTransData.V = _Cam->MatView();
		room->m_MoveTransData.P = _Cam->MatProj();
		room->m_MoveTransData.WV = room->m_MoveTransData.W * room->m_MoveTransData.V;
		room->m_MoveTransData.VP = _Cam->MatViewProj();
		room->m_MoveTransData.WVP = room->m_MoveTransData.W * room->m_MoveTransData.V * room->m_MoveTransData.P;

		if (true == CheckRange(room->m_TransData))
		{
			room->RD->On();
		}
		else
		{
			room->RD->Off();
		}

		if (true == room->b_MoveOn && true == CheckRange(room->m_MoveTransData))
		{
			room->RD_Move->On();
		}
		else
		{
			room->RD_Move->Off();
		}

		room->m_TransData.AllTP();
		room->m_MoveTransData.AllTP();
	}
}

void GameMiniMap::ChangeMinmapRect(const GameRect& _Other)
{
	MinimapRange = _Other;
}

GameMiniMap::ROADBLOCK* GameMiniMap::CreateRoad(__int64 _Key)
{
	__int64 Key = _Key;

	if (m_AllRoadBlock.end() != m_AllRoadBlock.find(Key))
	{
		return nullptr;
	}

	ROADBLOCK* NewBlock = new ROADBLOCK(nullptr);
	NewBlock->m_Key.TileKey = Key;

	std::pair<std::map<__int64, ROADBLOCK*>::iterator, bool> result = m_AllRoadBlock.insert(std::map<__int64, ROADBLOCK*>::value_type(Key, NewBlock));
	if (false == result.second)
	{
		ASSERTMSG;
	}

	m_AllRoadBlockVec.push_back(NewBlock);
	GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewBlock->m_TransData);
	RD->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
	RD->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
	RD->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
	RD->TEX(L"SpriteTex", m_RoadSprite[(int)RoadSpriteName::hall_dim]->Tex(), GameShader::SHADER_TYPE::ST_PS);
	RD->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
	NewBlock->RD = RD;

	return NewBlock;
}

void GameMiniMap::Clear()
{
	std::map<__int64, ROADBLOCK*>::iterator Start = m_AllRoadBlock.begin();
	std::map<__int64, ROADBLOCK*>::iterator End = m_AllRoadBlock.end();

	for (; Start != End; ++Start)
	{
		Start->second->RD->Death();
		delete Start->second;
		Start->second = nullptr;
	}

	m_AllRoadBlock.clear();
	m_AllRoadBlockVec.clear();

	std::map<__int64, ROOM*>::iterator StartRoom = m_AllRoom.begin();
	std::map<__int64, ROOM*>::iterator EndRoom = m_AllRoom.end();


	for (; StartRoom != EndRoom; ++StartRoom)
	{
		StartRoom->second->RD->Death();
		delete StartRoom->second;
		StartRoom->second = nullptr;
	}

	m_AllRoom.clear();
	m_AllRoomVec.clear();

	if (nullptr != m_MinimapIndicator)
	{
		m_MinimapIndicator->RD->Death();
		delete m_MinimapIndicator;
		m_MinimapIndicator = nullptr;
	}

	m_RenderList.clear();
}

const int ROAD_LENGTH = 7;

int GameMiniMap::MakeRandomMap(const int _RoomCount)
{
	if (_RoomCount < 1)
	{
		return 0;
		ASSERTMSG;
	}

	Clear();
	SpriteLoad();



	SKEY startKey;
	startKey.TileKey = CreateKey(0, 0);
	ROADBLOCK* start = CreateRoad(startKey.TileKey);
	ROADBLOCK* temp = nullptr;
	ROOM* startRoom = nullptr;
	ROOM* tempRoom = nullptr;

	{
		ROOM* NewRoom = new ROOM();
		NewRoom->m_Key = startKey;
		NewRoom->m_RoomSpriteName = RoomSpriteName::room_entrance;
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_TransData);
		NewRoom->RD = RD;
		RD->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::room_entrance]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);

		GameRenderData* RD_Move = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_MoveTransData);
		RD_Move->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD_Move->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::moving_room]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD_Move = RD_Move;
		NewRoom->RD_Move->Off();

		NewRoom->m_SamePosRoad = start;
		startRoom = NewRoom;

		std::pair<std::map<__int64, ROOM*>::iterator, bool> result = m_AllRoom.insert(std::map<__int64, ROOM*>::value_type(NewRoom->m_Key.TileKey, NewRoom));
		if (false == result.second)
		{
			RD->Death();
			delete NewRoom;
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		m_AllRoomVec.push_back(NewRoom);
	}

	int rotateCount = 1;
	int way = 0;
	int death_count = 4;
	int manual_way = 0;

	const ROADBLOCK* roomStarPtr = nullptr;

	for (size_t i = 0; i < _RoomCount; i++)
	{
		way = rand() % 4;
		death_count = 4;
		for (size_t i = 0; i < ROAD_LENGTH; i++)
		{
			//i == 2,3,4,5가 실제로 밖에 나와있게 된다.
			if (i == 0 && temp != nullptr)
			{
				SKEY temp = start->m_Key + ArrCheck[way] * ROAD_LENGTH;
				if (m_AllRoadBlock.end() != m_AllRoadBlock.find((start->m_Key + ArrCheck[way] * ROAD_LENGTH).TileKey))
				{
					/*rand가 실패하면 0~3까지 돌면서 들어갈수 있는 길로 들어간다.*/
					way = manual_way;
					--i;
					--death_count;
					++manual_way;
					if (death_count < 1 || way > 3)
					{
						if (nullptr == m_MinimapIndicator)
						{
							goto makeMinimapTray;
						}

						return (int)m_AllRoomVec.size();
					}
					continue;
				}
			}

			temp = CreateRoad((start->m_Key + ArrCheck[way]).TileKey);
			if (temp == nullptr)
			{
				ASSERTMSG;
			}
			start->Next = temp;
			temp->Prev = start;
			start = temp;


			if (0 == i)
			{
				switch (way)
				{
				case 0:
					startRoom->m_RoadBlock4Way[0] = temp;
					break;
				case 1:
					startRoom->m_RoadBlock4Way[1] = temp;
					break;
				case 2:
					startRoom->m_RoadBlock4Way[2] = temp;
					break;
				case 3:
					startRoom->m_RoadBlock4Way[3] = temp;
					break;
				default:
					ASSERTMSG;
					break;
				}
			}
		}

		if (m_AllRoom.find(temp->m_Key.TileKey) != m_AllRoom.end())
		{
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		ROOM* NewRoom = new ROOM();
		NewRoom->GetRandomName();
		NewRoom->m_Key = temp->m_Key;
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_TransData);
		RD->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::room_unknown]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD = RD;

		GameRenderData* RD_Move = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_MoveTransData);
		RD_Move->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD_Move->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::moving_room]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD_Move = RD_Move;
		NewRoom->RD_Move->Off();

		NewRoom->m_SamePosRoad = temp;



		switch (way)
		{
		case 0:
			NewRoom->m_RoadBlock4Way[2] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[2] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 1:
			NewRoom->m_RoadBlock4Way[3] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[3] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 2:
			NewRoom->m_RoadBlock4Way[0] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[0] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 3:
			NewRoom->m_RoadBlock4Way[1] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[1] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		default:
			ASSERTMSG;
			break;
		}

		std::pair<std::map<__int64, ROOM*>::iterator, bool> result = m_AllRoom.insert(std::map<__int64, ROOM*>::value_type(NewRoom->m_Key.TileKey, NewRoom));
		if (false == result.second)
		{
			RD->Death();
			delete NewRoom;
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		m_AllRoomVec.push_back(NewRoom);
		startRoom = NewRoom;
	}

	//생성된 방에서 모자란 길을 이어준다.
	{
		ROOM* ptr_Room;
		ROADBLOCK* start = nullptr;
		ROADBLOCK* temp = nullptr;
		std::map<__int64, ROOM*>::iterator NextRoom;

		for (int i = 0; i < m_AllRoomVec.size(); i++)
		{
			ptr_Room = m_AllRoomVec[i];
			start = ptr_Room->m_SamePosRoad;

			for (int way = 0; way < 4; way++)
			{
				NextRoom = m_AllRoom.find((ptr_Room->m_Key + ArrCheck[way] * ROAD_LENGTH).TileKey);
				if (m_AllRoom.end() != NextRoom)
				{
					ptr_Room->m_Room4Way[way] = NextRoom->second;
					if (nullptr == ptr_Room->m_RoadBlock4Way[way])
					{
						for (int k = 1; k < ROAD_LENGTH; k++)
						{
							temp = CreateRoad((start->m_Key + ArrCheck[way]).TileKey);
							if (nullptr == temp)
							{
								ASSERTMSG;
							}
							if (k == 1)
							{
								m_AllRoomVec[i]->m_RoadBlock4Way[way] = temp;
							}
							start->Next = temp;
							temp->Prev = start;
							start = temp;
						}

						start = ptr_Room->m_SamePosRoad;
						switch (way)
						{
						case 0:
							NextRoom->second->m_RoadBlock4Way[2] = temp;
							break;
						case 1:
							NextRoom->second->m_RoadBlock4Way[3] = temp;
							break;
						case 2:
							NextRoom->second->m_RoadBlock4Way[0] = temp;
							break;
						case 3:
							NextRoom->second->m_RoadBlock4Way[1] = temp;
							break;
						default:
							ASSERTMSG;
							break;
						}
					}
				}
			}
		}
	}

makeMinimapTray:
	{
		m_MinimapIndicator = new MINIMAPTRAY();
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", m_MinimapIndicator->m_TransData);
		m_MinimapIndicator->RD = RD;
		RD->SMP(L"LSMP", L"LSMP");
		RD->SMP(L"PSMP", L"PSMP");
		RD->CB(L"TEXCOLOR", m_Color, true);
		RD->TEX(L"SpriteTex", m_TrayResolve->Tex());
		RD->CB("TEXCUT", m_CutData, true);
	}

	return (int)m_AllRoomVec.size();
}



int GameMiniMap::MakeOrderMap(const int _RoomCount, const int* _WayArr, const int _ArrLength)
{
	if (_RoomCount < 1 || _RoomCount != _ArrLength)
	{
		return 0;
		ASSERTMSG;
	}

	Clear();
	SpriteLoad();



	SKEY startKey;
	startKey.TileKey = CreateKey(0, 0);
	ROADBLOCK* start = CreateRoad(startKey.TileKey);
	ROADBLOCK* temp = nullptr;
	ROOM* startRoom = nullptr;
	ROOM* tempRoom = nullptr;

	{
		ROOM* NewRoom = new ROOM();
		NewRoom->m_Key = startKey;
		NewRoom->m_RoomSpriteName = RoomSpriteName::room_entrance;
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_TransData);
		NewRoom->RD = RD;
		RD->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::room_entrance]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);

		GameRenderData* RD_Move = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_MoveTransData);
		RD_Move->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD_Move->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::moving_room]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD_Move = RD_Move;
		NewRoom->RD_Move->Off();

		NewRoom->m_SamePosRoad = start;
		startRoom = NewRoom;

		std::pair<std::map<__int64, ROOM*>::iterator, bool> result = m_AllRoom.insert(std::map<__int64, ROOM*>::value_type(NewRoom->m_Key.TileKey, NewRoom));
		if (false == result.second)
		{
			RD->Death();
			delete NewRoom;
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		m_AllRoomVec.push_back(NewRoom);
	}

	int rotateCount = 1;
	int way = 0;
	int death_count = 4;
	int manual_way = 0;

	const ROADBLOCK* roomStarPtr = nullptr;

	for (size_t i = 0; i < _RoomCount; i++)
	{
		way = _WayArr[i];
		death_count = 4;
		for (size_t i = 0; i < ROAD_LENGTH; i++)
		{
			//i == 2,3,4,5가 실제로 밖에 나와있게 된다.
			if (i == 0 && temp != nullptr)
			{
				SKEY temp = start->m_Key + ArrCheck[way] * ROAD_LENGTH;
				if (m_AllRoadBlock.end() != m_AllRoadBlock.find((start->m_Key + ArrCheck[way] * ROAD_LENGTH).TileKey))
				{
					/*rand가 실패하면 0~3까지 돌면서 들어갈수 있는 길로 들어간다.*/
					way = manual_way;
					--i;
					--death_count;
					++manual_way;
					if (death_count < 1 || way > 3)
					{
						if (nullptr == m_MinimapIndicator)
						{
							goto makeMinimapTray;
						}

						return (int)m_AllRoomVec.size();
					}
					continue;
				}
			}

			temp = CreateRoad((start->m_Key + ArrCheck[way]).TileKey);
			if (temp == nullptr)
			{
				ASSERTMSG;
			}
			start->Next = temp;
			temp->Prev = start;
			start = temp;


			if (0 == i)
			{
				switch (way)
				{
				case 0:
					startRoom->m_RoadBlock4Way[0] = temp;
					break;
				case 1:
					startRoom->m_RoadBlock4Way[1] = temp;
					break;
				case 2:
					startRoom->m_RoadBlock4Way[2] = temp;
					break;
				case 3:
					startRoom->m_RoadBlock4Way[3] = temp;
					break;
				default:
					ASSERTMSG;
					break;
				}
			}
		}

		if (m_AllRoom.find(temp->m_Key.TileKey) != m_AllRoom.end())
		{
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		ROOM* NewRoom = new ROOM();
		NewRoom->GetRandomName();
		NewRoom->m_Key = temp->m_Key;
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_TransData);
		RD->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::room_unknown]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD = RD;

		GameRenderData* RD_Move = CreateRenderData(L"2DRECT", L"2DSPRITE", NewRoom->m_MoveTransData);
		RD_Move->SMP(L"LSMP", L"LSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->SMP(L"PSMP", L"PSMP", GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB(L"TEXCOLOR", m_Color, true, GameShader::SHADER_TYPE::ST_PS);
		RD_Move->TEX(L"SpriteTex", m_RoomSprite[(int)RoomSpriteName::moving_room]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		RD_Move->CB("TEXCUT", m_CutData, true, GameShader::SHADER_TYPE::ST_VS);
		NewRoom->RD_Move = RD_Move;
		NewRoom->RD_Move->Off();

		NewRoom->m_SamePosRoad = temp;



		switch (way)
		{
		case 0:
			NewRoom->m_RoadBlock4Way[2] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[2] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 1:
			NewRoom->m_RoadBlock4Way[3] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[3] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 2:
			NewRoom->m_RoadBlock4Way[0] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[0] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		case 3:
			NewRoom->m_RoadBlock4Way[1] = temp->Prev;
			m_AllRoomVec[m_AllRoomVec.size() - 1]->m_Room4Way[way] = NewRoom;
			NewRoom->m_Room4Way[1] = m_AllRoomVec[m_AllRoomVec.size() - 1];
			break;
		default:
			ASSERTMSG;
			break;
		}

		std::pair<std::map<__int64, ROOM*>::iterator, bool> result = m_AllRoom.insert(std::map<__int64, ROOM*>::value_type(NewRoom->m_Key.TileKey, NewRoom));
		if (false == result.second)
		{
			RD->Death();
			delete NewRoom;
			ASSERTMSG;
			return (int)m_AllRoomVec.size();
		}
		m_AllRoomVec.push_back(NewRoom);
		startRoom = NewRoom;
	}

	//생성된 방에서 모자란 길을 이어준다.
	{
		ROOM* ptr_Room;
		ROADBLOCK* start = nullptr;
		ROADBLOCK* temp = nullptr;
		std::map<__int64, ROOM*>::iterator NextRoom;

		for (int i = 0; i < m_AllRoomVec.size(); i++)
		{
			ptr_Room = m_AllRoomVec[i];
			start = ptr_Room->m_SamePosRoad;

			for (int way = 0; way < 4; way++)
			{
				NextRoom = m_AllRoom.find((ptr_Room->m_Key + ArrCheck[way] * ROAD_LENGTH).TileKey);
				if (m_AllRoom.end() != NextRoom)
				{
					ptr_Room->m_Room4Way[way] = NextRoom->second;
					if (nullptr == ptr_Room->m_RoadBlock4Way[way])
					{
						for (int k = 1; k < ROAD_LENGTH; k++)
						{
							temp = CreateRoad((start->m_Key + ArrCheck[way]).TileKey);
							if (nullptr == temp)
							{
								ASSERTMSG;
							}
							if (k == 1)
							{
								m_AllRoomVec[i]->m_RoadBlock4Way[way] = temp;
							}
							start->Next = temp;
							temp->Prev = start;
							start = temp;
						}

						start = ptr_Room->m_SamePosRoad;
						switch (way)
						{
						case 0:
							NextRoom->second->m_RoadBlock4Way[2] = temp;
							break;
						case 1:
							NextRoom->second->m_RoadBlock4Way[3] = temp;
							break;
						case 2:
							NextRoom->second->m_RoadBlock4Way[0] = temp;
							break;
						case 3:
							NextRoom->second->m_RoadBlock4Way[1] = temp;
							break;
						default:
							ASSERTMSG;
							break;
						}
					}
				}
			}
		}
	}

makeMinimapTray:
	{
		m_MinimapIndicator = new MINIMAPTRAY();
		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", m_MinimapIndicator->m_TransData);
		m_MinimapIndicator->RD = RD;
		RD->SMP(L"LSMP", L"LSMP");
		RD->SMP(L"PSMP", L"PSMP");
		RD->CB(L"TEXCOLOR", m_Color, true);
		RD->TEX(L"SpriteTex", m_TrayResolve->Tex());
		RD->CB("TEXCUT", m_CutData, true);
	}

	return (int)m_AllRoomVec.size();
}

void GameMiniMap::ChangeIndicatorPos(const SKEY& _Key)
{
	m_MinimapIndicator->m_Key = _Key;
}
