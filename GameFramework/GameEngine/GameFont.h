#pragma once
#include <GameMath.h>
#include <GameRes.h>
#include "FW1FontWrapper.h"
#include <list>

#pragma comment(lib, "FW1FontWrapper.lib")

class GameFont : public GameRes<GameFont>
{
private:
	//FontFactory�� ������ ������ �ʿ䰡 ����.
	static IFW1Factory* m_pFontFactory;
	//�۲��� ������ ���� ���
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

