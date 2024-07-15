#pragma once
#include "GameRenderer.h"
#include "GameSprite.h"
#include <vector>
#include <map>

//MINIMAPRENDER->Create()���·� �߰�
class SMAPSCENECOM;
class GameMap;
class GameMiniMap : public GameRenderer
{
public:
	friend SMAPSCENECOM;

	enum class RoomSpriteName
	{
		room_battle,
		room_boss,
		room_curio,
		room_empty,
		room_entrance,
		room_treasure,
		room_unknown,
		moving_room,
		RoomMax
	};

	enum class RoadSpriteName
	{
		hall_clear,
		hall_dark,
		hall_dim,
		RoadMax
	};

	enum class ROOMTYPE
	{
		START,
		BOSS,
		RoomTypeMAX,
	};

	//class MAPCONCEPTDATA
	//{
	//public:
	//	//�Ա��� ���̴� �̹���
	//	HGAMESTRING DOORIMAGE;
	//	int RoomBackGroundCount;
	//	//���������� ���̴� �̹���
	//	HGAMESTRING ROADIMAGE[255];
	//	int RoomBackGroundCount;
	//	HGAMESTRING TOPSPRITE;
	//	HGAMESTRING BOTSPRITE;

	//	HGAMESTRING ROADBACKGROUND1;
	//	HGAMESTRING ROADBACKGROUND2;
	//	HGAMESTRING ROADBACKGROUND2;

	//	HGAMESTRING ROOMBACKGROUND[255];
	//	HGAMESTRING BOSSROOM;
	//};

public:

	class SKEY
	{
	public:
		union
		{
			struct
			{
				int X;
				int Y;
			};

			__int64 TileKey;
		};

		SKEY operator *(const int _Other)
		{
			SKEY temp;
			temp.TileKey = this->TileKey;
			temp.X *= _Other;
			temp.Y *= _Other;
			return temp;
		}


		SKEY operator+(const SKEY& _Other)
		{
			SKEY temp;
			temp.TileKey = this->TileKey;
			temp.X += _Other.X;
			temp.Y += _Other.Y;
			return temp;
		}

		SKEY operator-(const SKEY& _Other)
		{
			SKEY temp;
			temp.TileKey = this->TileKey;
			temp.X -= _Other.X;
			temp.Y -= _Other.Y;
			return temp;
		}

		SKEY& operator+=(const SKEY& _Other)
		{
			this->X += _Other.X;
			this->Y += _Other.Y;
			return *this;
		}

		bool operator==(const SKEY& _Other)
		{
			return (this->X == _Other.X) && (this->Y == _Other.Y);
		}

		SKEY() : X(0), Y(0)
		{
		}

		SKEY(const SKEY& _Other)
		{
			X = _Other.X;
			Y = _Other.Y;
			TileKey = _Other.TileKey;
		}

		SKEY(int _X, int _Y) : X(_X), Y(_Y)
		{

		}
	};
private:

	enum RoadDIR
	{
		L,
		T,
		R,
		B,
		MAX,
	};

public:
	class ROADBLOCK
	{
	private:
		friend GameMiniMap;

	public:
		//�ڱ� ��ġ�� ������ ����
		SKEY   m_Key;

		//������ ���� ��ġ�� ����Ű�� ����
		ROADBLOCK* Next;
		ROADBLOCK* Prev;

		GameRenderData* RD;
		CTransData m_TransData;
		RoadSpriteName m_Name;
	public:
		ROADBLOCK(GameRenderData* _RD) : RD(_RD)
		{

		}

		void SpriteChage(RoadSpriteName _Name)
		{
			m_Name = _Name;
		}
	};


public:
	class ROOM
	{
	private:
		friend GameMiniMap;

	public:
		SKEY   m_Key;
		ROOMTYPE m_Type;
		ROOM* m_Room4Way[4];
		ROADBLOCK* m_RoadBlock4Way[4];
		ROADBLOCK* m_SamePosRoad;

		bool b_MoveOn;
		GameRenderData* RD;
		GameRenderData* RD_Move;
		CTransData m_TransData;
		CTransData m_MoveTransData;

		RoomSpriteName m_RoomSpriteName;

	public:
		ROOM()
		{
		}

		void GetRandomName()
		{
			m_RoomSpriteName = (RoomSpriteName)(rand() % (int)RoomSpriteName::RoomMax);
		}

		void Sprite(RoomSpriteName _Name)
		{
			if ((int)_Name < 0 && (int)_Name >(int)RoomSpriteName::RoomMax)
			{
				ASSERTMSG;
			}
			RD->TEX(L"SpriteTex", m_RoomSprite[(int)_Name]->Tex(), GameShader::SHADER_TYPE::ST_PS);
		}

		void Move_On()
		{
			b_MoveOn = true;
		}

		void Move_Off()
		{
			b_MoveOn = false;
		}
		//HGAMESTRING BACKGROUND;
		//MONSTERDATA* MonsterList[4];
	};

	class MINIMAPTRAY
	{
	private:
		friend GameMiniMap;

	public:
		SKEY m_Key;
		GameRenderData* RD;
		CTransData m_TransData;

	public:
		MINIMAPTRAY() : m_Key({ 0,0 })
		{

		}
	};
	//class GAMEMAP
	//{
	//public:
	//	MAPCONCEPTDATA ArrMapConcept[4]
	//		static void Init();
	//	static void Create(GameScene _Scene, int _Concept, int RoomCount);
	//	static void Clear();

	//	ROOM* m_StartRoom;
	//	ROOM* m_BossRoom;

	//	list<ROOM*>
	//		map<__int64, ROOM>

	//private:
	//	static void CreateBlock(ROOM* Src, ROOM* Dest, DIR _Dir)
	//	{
	//		bool ó�������̳� ?
	//			bool �������ִ��� ?
	//			int
	//	}



	//public:
	//	GAMEMAP();
	//	~GAMEMAP();
	//};

	//�� �̴ϸ� �����ܵ��� �� ������ �ϳ��� ��� ����.
	//�ϴ� �̹����� �ϳ��� ��� �� �ְ� ���������� �����ϰ� �־�����.


private:
	friend ROADBLOCK;
	friend ROOM;
	friend MINIMAPTRAY;
	friend GameMap;

	CVector m_Color;
	CVector m_CutData;

	static SPtr<GameSprite> m_RoomSprite[8];
	static SPtr<GameSprite> m_RoadSprite[3];
	SPtr<GameSprite> m_TrayResolve;
	static SKEY ArrCheck[4];
	static RoadDIR ArrTILE4WAYDIR[4];

public:
	std::map<__int64, ROADBLOCK*> m_AllRoadBlock;
	std::vector<ROADBLOCK*> m_AllRoadBlockVec;

	std::map<__int64, ROOM*> m_AllRoom;
	std::map<__int64, ROOM*>::iterator m_RoomIter;
	std::vector<ROOM*> m_AllRoomVec;

	static GameRect MinimapRange;

public:
	MINIMAPTRAY* m_MinimapIndicator;
	CVector m_MinimapIndicatorPos;

public:
	GameMiniMap();
	~GameMiniMap();

public:
	void Render(SPtr<GameCam> _Cam) override;

	void RoadBlockSprite(const std::vector<GameString>& _Name);
	void RoomSprite(const std::vector<GameString>& _Name);

	int MakeRandomMap(int _RoomCount);
	int MakeOrderMap(const int _RoomCount, const int* _WayArr, const int _ArrLength);
	void ChangeMinmapRect(const GameRect& _Other);
	void ChangeIndicatorPos(const SKEY& _Key);

private:
	void CalTransData(SPtr<GameCam> _Cam);
	void SpriteCheck();
	void SpriteLoad();
	void Clear();
	bool CheckRange(const CTransData& _Trans);

	__int64 CreateKey(int _X, int _Y)
	{
		SKEY Key = SKEY(_X, _Y);
		return Key.TileKey;
	}

	ROADBLOCK* CreateRoad(const CVector& _Pos)
	{
		return CreateRoad((int)std::roundf(_Pos.x), (int)std::roundf(_Pos.y));
	}

	ROADBLOCK* CreateRoad(const int _X, const int _Y)
	{
		return CreateRoad(CreateKey(_X, _Y));
	}

	ROADBLOCK* CreateRoad(const __int64 _Key);


};

