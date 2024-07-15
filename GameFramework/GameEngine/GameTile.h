#pragma once
#include "GameRenderer.h"
#include "GameSprite.h"
#include <list>


class GameTile : public GameRenderer
{
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

		SKEY& operator+=(const SKEY& _Other)
		{
			X += _Other.X;
			Y += _Other.Y;
			return *this;
		}

		SKEY() : X(0), Y(0)
		{
		}

		SKEY(int _X, int _Y) : X(_X), Y(_Y)
		{

		}
	};

	enum TILEDIR
	{
		L,
		LT,
		T,
		RT,
		R,
		RB,
		B,
		LB,
		MAX,
	};


	struct TILEINDEXDATA
	{
	public:
		bool Check[8];
		int Index;
	};

	static SKEY ArrCheck[8];
	static TILEDIR ArrTILE4WAYDIR[4];
	static TILEINDEXDATA TileIndex[49];

	class Tile
	{
	public:
		// 몇번째로 그릴거냐.
		SKEY m_Key;
		CTransData m_TransData;
		unsigned int m_Index;
		GameRenderData* RD;

		Tile(int _Index, GameRenderData* _RD) : RD(_RD), m_Index(_Index)
		{

		}
	};

private:
	SPtr<GameSprite> m_Sprite;
	CVector m_Color;
	std::map<__int64, Tile*> m_AllTile;
	std::list<Tile*> m_AllTileList;

public:
	GameTile();
	~GameTile();

public:
	void Sprite(const GameString& _Name) {
		m_Sprite = GameSprite::Find(_Name);

		if (nullptr == m_Sprite)
		{
			ASSERTMSG;
		}
	}

	SPtr<GameSprite> Sprite() { return m_Sprite; }



	__int64 CreateKey(int _X, int _Y)
	{
		SKEY Key = SKEY(_X, _Y);
		return Key.TileKey;
	}

	void CreateTile(CVector _Pos, unsigned int _Index = -1)
	{
		CreateTile((int)std::roundf(_Pos.x), (int)std::roundf(_Pos.y), _Index);
	}

	void CreateTile(int _X, int _Y, unsigned int _Index = -1)
	{
		CreateTile(CreateKey(_X, _Y), _Index);
	}

	void CreateTile(__int64 _Key, unsigned int _Index = -1)
	{
		__int64 Key = _Key;

		if (m_AllTile.end() != m_AllTile.find(Key))
		{
			return;
		}

		Tile* NewTile = new Tile(0, nullptr);
		NewTile->m_Key.TileKey = Key;

		m_AllTile.insert(std::map<__int64, Tile*>::value_type(Key, NewTile));
		m_AllTileList.push_back(NewTile);

		GameRenderData* RD = CreateRenderData(L"2DRECT", L"2DSPRITE", NewTile->m_TransData);
		RD->SMP(L"LSMP", L"LSMP");
		RD->SMP(L"PSMP", L"PSMP");
		RD->CB(L"TEXCOLOR", m_Color, true);
		RD->TEX("SpriteTex", m_Sprite->Tex());
		NewTile->RD = RD;

		if (-1 == _Index)
		{
			CalTileTest(Key);

			SKEY CheckKey;
			for (size_t i = 0; i < MAX; i++)
			{
				if (i == TILEDIR::RT)
				{
					int a = 0;
				}

				CheckKey.TileKey = Key;
				CheckKey += ArrCheck[i];

				CalTileTest(CheckKey.TileKey);
			}
		}
		else {
			NewTile->m_Index = _Index;
			NewTile->RD->CB("TEXCUT", m_Sprite->CutData(NewTile->m_Index));
		}
	}

	void Render(SPtr<GameCam> _Cam) override;

	void CalTileTest(__int64 Key);

	void Save(const GameString& _Path);
	void Load(const GameString& _Path);

	void Clear();
};


