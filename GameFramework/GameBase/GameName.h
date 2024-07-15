#pragma once
// .h�� �پ������� c���
// �ƴϸ� c++ ���
// ���������� �̸��� �Ȱ���.
#include "GameString.h"

// ���߻�ӿ��� virtual�� �ϸ�
// ���߻���� ����� �Ǵ� Ŭ������
// �̸��� string���� �����Ѵ�(�� string�� ���� �ٲ�� �ִ�.)
// �ִ��� ������ �����ؾ��Ѵ�.
// virtual�� ���� �����Ƿ� �ܵ� ������� �����������
// SREF�� ���� ����� �ؾ� �Ҹ��ڰ� �θ�Ŭ���������� ã�ư��� �һ�簡 �Ͼ�� �ʴ´�.

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