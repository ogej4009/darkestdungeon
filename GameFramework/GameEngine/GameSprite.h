#pragma once
#include "GameDevice.h"
#include "GameShader.h"
#include <GameRes.h>
#include "GameTexture.h"

class GameSprite : public GameRes<GameSprite>
{
private:
	SPtr<GameTexture> m_Tex;
	std::vector<CVector> m_CutPos;
	std::vector<std::vector<CVector>> m_CutPos2D;

public:
	GameSprite();
	~GameSprite();

public:
	SPtr<GameTexture> Tex()
	{
		return m_Tex;
	}

	static SPtr<GameSprite> Create(const GameString& _TexPath, size_t _W, size_t _H)
	{
		GameString Name = _TexPath.PathData().ArrFullFile;
		GameSprite* NewRes = CreateRes(Name);
		NewRes->CreateSprite(Name, _W, _H);
		return NewRes;
	}

	static SPtr<GameSprite> Create(const GameString& _SpriteName, const GameString& _TexName, size_t _W, size_t _H)
	{
		GameSprite* NewRes = CreateRes(_SpriteName);
		NewRes->CreateSprite(_TexName, _W, _H);
		return NewRes;
	}

	bool CreateSprite(const GameString& _TexName, size_t _W, size_t _H);

	void ChangeCutData(std::vector<CVector> _UvData)
	{
		m_CutPos = _UvData;
	}


	CVector CutData(size_t _Index)
	{
		if (_Index < 0)
		{
			ASSERTMSG;
		}

		if (_Index >= m_CutPos.size())
		{
			ASSERTMSG;
		}

		return m_CutPos[_Index];
	}

};

