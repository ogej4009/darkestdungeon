#pragma once
#include "GameLogic.h"
#include <vector>
#include <GameMath.h>

class GameJsonReader : public GameLogic
{
public:
	class JsonData
	{
	public:
		GameString m_Name;
		unsigned int m_X;
		unsigned int m_Y;
		unsigned int m_Width;
		unsigned int m_Height;
	};

private:
	//�ѹ� UV������ ĳ�� ������ �༮���̶� �����Ҵ��� ����?
	std::vector<JsonData> m_Data;
	std::vector<CVector> m_UvData;

public:
	unsigned int m_Count;
	unsigned int m_XCount;
	unsigned int m_YCount;
	unsigned int m_TexWidth;
	unsigned int m_TexHeight;
	float m_UvX;
	float m_UvY;

public:
	GameJsonReader();
	~GameJsonReader();

public:
	void ParseJson(const GameString& _FilePath);
	std::vector<CVector> MakeUvData();
	unsigned int Count()
	{
		return m_Count;
	}

};

