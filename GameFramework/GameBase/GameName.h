#pragma once
// .h가 붙어있으면 c헤더
// 아니면 c++ 헤더
// 엿같은것이 이름이 똑같다.
#include "GameString.h"

// 다중상속에서 virtual을 하면
// 다중상속의 대상이 되는 클래스로
// 이름만 string으로 관리한다(단 string은 추후 바뀔수 있다.)
// 최대한 가볍게 유지해야한다.
// virtual도 쓰지 않으므로 단독 상속으로 사용하지말고
// SREF와 같이 상속을 해야 소멸자가 부모클래스쪽으로 찾아가는 불상사가 일어나지 않는다.

class GameName
{
private:
	GameString m_Name;

public:
	std::wstring StringName()
	{
		return m_Name;
	}

	GameString SStringName()
	{
		return m_Name;
	}


	const wchar_t* Name()
	{
		return m_Name;
	}

	void Name(const wchar_t* _Name)
	{
		m_Name = _Name;
	}

	void Name(std::wstring& _Name)
	{
		m_Name = _Name;
	}

	void Name(const GameString& _Name)
	{
		m_Name = _Name;
	}


public:
	GameName() {}
	~GameName() {}
};