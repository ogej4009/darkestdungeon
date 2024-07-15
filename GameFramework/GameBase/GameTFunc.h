#pragma once
#include <map>


class GameTFunc
{
public:
	GameTFunc();
	~GameTFunc();

public:
	template<typename T>
	static void ZERO(T& _Value)
	{
		//시작위치, 채울 값, 사이즈
		memset(&_Value, 0, sizeof(T));
	}

	template<typename FINDKEY, typename MAPKEY, typename MAPVALUE>
	static MAPVALUE MapFindPointer(std::map<MAPKEY, MAPVALUE>& _Map, FINDKEY& _Key)
	{
		typename std::map<MAPKEY, MAPVALUE>::iterator FindIter = _Map.find(_Key);

		if (FindIter == _Map.end())
		{
			return nullptr;
		}

		return FindIter->second;
	}

};

