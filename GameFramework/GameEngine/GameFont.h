#pragma once
#include <GameMath.h>
#include <GameRes.h>
#include "FW1FontWrapper.h"
#include <list>

#pragma comment(lib, "FW1FontWrapper.lib")

class GameFont : public GameRes<GameFont>
{
private:
	//FontFactory는 여러번 생성될 필요가 없다.
	static IFW1Factory* m_pFontFactory;
	//글꼴을 가지고 있을 멤버
	IFW1FontWrapper* m_pFontWrapper;

public:
	static std::list<GameString> m_FontList;
	static std::list<GameString>::iterator m_FontListIter;

public:
	GameFont();
	~GameFont();

public:
	static bool Create(const GameString& _FontName)
	{
		GameFont* NewRes = CreateRes(_FontName);
		return NewRes->CreateGameFont(_FontName);
	}

	bool CreateGameFont(const GameString& _FontName);
	void DrawUI(const GameString& _Text, float _Size, CVector _Pos, CVector _Color, FW1_TEXT_FLAG _Flag = FW1_TOP);
};

