#include "GameJsonReader.h"
#include "GameDic.h"
#include <GameMath.h>


GameJsonReader::GameJsonReader()
	:m_Count(0),
	m_XCount(0),
	m_YCount(0)
{

}

GameJsonReader::~GameJsonReader()
{
	if (m_Data.size() != 0)
	{
		m_Data.clear();
	}
	
	if (m_UvData.size() != 0)
	{
		m_UvData.clear();
	}
}

void GameJsonReader::ParseJson(const GameString& _FilePath)
{
	GameFile jsonFile = GameFile(_FilePath, L"rb");

	GameString innerString = jsonFile.GetFileToText();
	size_t lastWord = innerString.m_Str.find_last_of(L"]");
	innerString.m_Str.replace(lastWord + 1, innerString.m_Str.size() - lastWord, L"");

	m_Data.clear();

	//처음으로 찾은 name,x,y,width의 인덱스를 리턴받고
	//그 다음 각각의 인덱스에서 " " 내부 데이터를 읽어오게 만들어보자. 
	std::vector<size_t> nameCount;
	std::vector<size_t> xCount;
	std::vector<size_t> yCount;
	std::vector<size_t> widthCount;
	std::vector<size_t> heightCount;

	JsonData jsonData = JsonData();

	nameCount = innerString.FindToCount(L"name");
	xCount = innerString.FindToCount(L"x");
	yCount = innerString.FindToCount(L"y");
	widthCount = innerString.FindToCount(L"width");
	heightCount = innerString.FindToCount(L"height");


	m_Count = (unsigned int)nameCount.size();
	m_Data.resize(m_Count);

	size_t end = 0;
	int index = 0;

	for (size_t i = 0; i < m_Count; i++)
	{
		end = innerString.m_Str.find(L"\"", nameCount[i] + 7);
		GameString indexStr = innerString.GetCutText(nameCount[i] + 10, end - 1);
		index = std::stoi(indexStr.m_Str.c_str());
		m_Data[index - 1].m_Name = innerString.GetCutText(nameCount[i] + 7, end - 1);

		end = innerString.m_Str.find(L"\"", xCount[i] + 2);
		m_Data[index - 1].m_X = std::stoi(innerString.GetCutText(xCount[i]+3, end - 2).m_Str.c_str());

		end = innerString.m_Str.find(L"\"", yCount[i] + 2);
		m_Data[index - 1].m_Y = std::stoi(innerString.GetCutText(yCount[i]+3, end-2).m_Str.c_str());

		end = innerString.m_Str.find(L"\"", widthCount[i] +7);
		m_Data[index - 1].m_Width = std::stoi(innerString.GetCutText(widthCount[i] + 7, end - 2).m_Str.c_str());

		end = innerString.m_Str.find(L"}", heightCount[i] + 8);
		m_Data[index - 1].m_Height = std::stoi(innerString.GetCutText(heightCount[i] + 8, end - 1).m_Str.c_str());
	}
}

std::vector<CVector> GameJsonReader::MakeUvData()
{
	if (m_Data.size() < 0)
	{
		//데이터가 없습니다.
		ASSERTMSG;
	}

	m_UvData.clear();

	int CalTexWidth = 0;
	int CalTexHeight = 0;
	int width = m_Data[0].m_Width;
	int height = m_Data[0].m_Height;

	for (auto& Data : m_Data)
	{
		if (CalTexWidth < (int)Data.m_X)
		{
			CalTexWidth = (int)Data.m_X;
		}

		if (CalTexHeight < (int)Data.m_Y)
		{
			CalTexHeight = (int)Data.m_Y;
		}
	}

	m_TexWidth = CalTexWidth + width;
	m_TexHeight = CalTexHeight + height;

	//x쪽으로 몇개 있는지 y쪽으로 몇개 있는지
	//최대값에서 연속적으로 빼다보면 padding값이 나온다.
	//그 패딩에 +1해주면 원하는 텍스쳐의 숫자가 나온다. 
	while (true)
	{
		if (CalTexWidth - width < 0)
		{
			m_XCount = ++CalTexWidth;
			break;
		}

		CalTexWidth -= width;
	}

	while (true)
	{
		if (CalTexHeight - height < 0)
		{
			m_YCount = ++CalTexHeight;
			break;
		}

		CalTexHeight -= height;
	}

	m_UvX =  1.0f / (float)m_TexWidth;
	m_UvY = 1.0f / (float)m_TexHeight;

	CVector tempData = CVector::ZERO;

	for (size_t i = 0; i < m_Count; ++i)
	{
		tempData.x = (float)m_Data[i].m_X / (float)m_TexWidth;
		tempData.y = (float)m_Data[i].m_Y / (float)m_TexHeight;
		//x쪽 카운트 
		tempData.z = m_UvX * width;
		//y쪽 카운트
		tempData.w = m_UvY * height;
		m_UvData.push_back(tempData);
	}

	return m_UvData;
}
