#pragma once
#include <typeinfo>

// Run-Time Type Information
// 실행도중 타입을 알아내는 것
// C#은 리플렉션 기능이 이걸 담당하는데
// C++의 RTTI는 어린애 수준의 밖에 안된다.
// C++ 20버전에서는 #include <meta.h>가 뭔가 해준데.

// 클래스명을 알아내는것, 클래스명 해시코드 가져오는 거
class GameRTTI
{
private:
	const type_info* m_TypeInfo;

public:
	GameRTTI();
	~GameRTTI();

public:
	const type_info* Type()
	{
		return m_TypeInfo;
	}

	bool Check(const GameRTTI& _Other)
	{
		return _Other.m_TypeInfo == m_TypeInfo;
	}

	bool Check(const type_info* _TypeInfo)
	{
		return m_TypeInfo == _TypeInfo;
	}

	template<typename T>
	bool Check()
	{
		return m_TypeInfo == &typeid(T);
	}
		
	void TypeSetting();

	void TypeSetting(const type_info* _TypeInfo)
	{
		m_TypeInfo = _TypeInfo;
	}

	template<typename TYPE>
	void TypeSetting()
	{
		m_TypeInfo = &typeid(TYPE);
	}
};

