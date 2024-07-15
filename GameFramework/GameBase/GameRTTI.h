#pragma once
#include <typeinfo>

// Run-Time Type Information
// ���൵�� Ÿ���� �˾Ƴ��� ��
// C#�� ���÷��� ����� �̰� ����ϴµ�
// C++�� RTTI�� ��� ������ �ۿ� �ȵȴ�.
// C++ 20���������� #include <meta.h>�� ���� ���ص�.

// Ŭ�������� �˾Ƴ��°�, Ŭ������ �ؽ��ڵ� �������� ��
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

