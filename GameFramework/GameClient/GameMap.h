#pragma once
#include "GameComponent.h"
#include <GameRef.h>
#include <GameMath.h>
#include <GameMiniMap.h>
#include <map>
#include <vector>

class GameSprite;
class GameImage;
class GameRenderMgr;
class GameMap : public GameComponent
{
private:
	friend GameScene;
	friend GameRenderMgr;
	friend GameActor;

	//이름을 받고 그 이름으로 TEX까지 만들어서 들고 있자.
	class DungeonSprite
	{
	private:
		friend GameMap;
		SPtr<GameSprite> m_DefaultImg[6];
		std::map<GameString, SPtr<GameSprite>> m_Corridor_Wall;
		std::map<GameString, SPtr<GameSprite>>::iterator m_Corridor_Wall_Iter;
		std::map<GameString, SPtr<GameSprite>> m_Room_Wall;
		std::map<GameString, SPtr<GameSprite>>::iterator m_Room_Iter;
		std::vector<SPtr<GameSprite>> m_DoorSprite;

	private:
		DungeonSprite();
		~DungeonSprite();
	};

	class DungeonWallBackRender
	{
	public:
		friend DungeonSprite;
		friend GameMap;
	public:
		SPtr<GameImage> m_Corridor_back;
		SPtr<GameImage> m_Corridor_mid;
		SPtr<GameImage> m_WallRender;

	public:
		DungeonWallBackRender() : m_Corridor_back(nullptr), m_Corridor_mid(nullptr), m_WallRender(nullptr)
		{

		}

	public:
		void RenderOn()
		{
			if (m_Corridor_back != nullptr && m_Corridor_mid != nullptr)
			{
				m_Corridor_back->On();
				m_Corridor_mid->On();
			}
			m_WallRender->On();
		}

		void RenderOff()
		{
			if (m_Corridor_back != nullptr && m_Corridor_mid != nullptr)
			{
				m_Corridor_back->Off();
				m_Corridor_mid->Off();
			}
			
			m_WallRender->Off();
		}
	};


private:
	enum MapState
	{
		Room,
		Road,
	};

	enum DefaultImg
	{
		corridor_bg,
		corridor_door,
		corridor_mid,
		endhall,
		foreground_bottom,
		foreground_top,
	};

	enum UISpirte
	{
		side_decor,
		panel_banner_controller,
		panel_hero,
		panel_map,
		focuscam_button,
		party_order_button,
		torch,
		panel_map_upper_piece,
		bottom_background,
		fadeCall,
		preFadeCall,
		Max
	};

	friend DungeonSprite;
	//던전 관련 모든 스프라이트 집합
	DungeonSprite* m_DungeonSprite;
	GameString m_CorridorPart_Name[6];
	//UI관련
	SPtr<GameImage> m_UIRender[12];
	SPtr<GameSprite> m_UISprite[11];
	//Fade관련
	float m_FadeTimer;
	float m_HalfFadeTimer;
	SPtr<GameImage> m_FadeImgRender;
	CVector m_FadeAlpha;
	//Room관련
	SPtr<GameImage> m_RoomRender;
	//Road Corridor 관련
	std::vector<DungeonWallBackRender*> m_BackRoadRender;
	MapState m_State;
	//Door 관련
	SPtr<GameImage> m_StartDoor;
	SPtr<GameImage> m_EndDoor;
	CVector m_StartDoorPos;
	CVector m_EndDoorPos;

private:
	GameMap();
	~GameMap();

public:
	bool MapLoad(GameString _Name);
	void DisplayMapAndUI();
	void FadeCall(SPtr<GameCam> _Cam, float _Duration = 1.0f);
	void ChangeFadePos(CVector _Pos);
	void Play_Road_Render();
	void Play_CryptRoom_Render(GameMiniMap::RoomSpriteName _RoomName = GameMiniMap::RoomSpriteName::RoomMax);
	void Play_TutorialRoom(GameMiniMap::RoomSpriteName _RoomName = GameMiniMap::RoomSpriteName::RoomMax);
	void OpenDoor(bool _is_StartDoor);
	void CloseDoor();
	float FadeTimer()
	{
		return m_FadeTimer;
	}

	float FadeHalfTimer()
	{
		return m_HalfFadeTimer;
	}
	//void MoveRoom();

private:
	void Init();
	void UpdatePrev();
	void Update();
	void UpdateNext();
	void DebugRender();
	void AllRenderOff();

};

