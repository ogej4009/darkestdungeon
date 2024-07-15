#include "GameString.h"

GameString::GameString()
{
}


GameString::~GameString()
{
}

GameString::GameString(const wchar_t* _Str) : m_Str(_Str) {}

GameString::GameString(const char* _Str)
	: m_Str(CA2W(_Str).m_psz)
{

}
GameString::GameString(const std::wstring& _Str) : m_Str(_Str) {}
GameString::GameString(const std::string& _Str) : m_Str(CA2W(_Str.c_str()).m_psz) {}


bool GameString::IsContains(const wchar_t* _Find)
{
	size_t Result = m_Str.find(_Find);

	if (Result == -1)
	{
		return false;
	}

	return true;
}

void GameString::Change(const wchar_t* _Prev, const wchar_t* _Next)
{
	// m_Str.FIN

	// m_Str.replace(_Prev, _Next);

	std::wstring Prev = _Prev;

	// �տ��� ���� ã�°�
	while (true)
	{
		size_t Result = m_Str.find(Prev);

		if (Result == -1)
		{
			break;
		}

		m_Str.replace(Result, Prev.size(), _Next);
	}

	// while (true)
	// {
	// }

}

void GameString::ChangeParent(const wchar_t* _Prev, const wchar_t* _Next)
{
	// m_Str.FIN

	// m_Str.replace(_Prev, _Next);

	std::wstring Prev = _Prev;

	// �տ��� ���� ã�°�
	size_t Result = m_Str.rfind(_Prev);
	if (Result == std::wstring::npos)
	{
		return;
	}

	m_Str.replace(Result, Prev.size(), _Next);
	// while (true)
	// {
	// }

}

bool GameString::ChangeToFirstOne(const wchar_t* _Prev, const wchar_t* _Next)
{
	std::wstring Prev = _Prev;

	// �տ��� ���� ã�°�
	size_t Result = m_Str.find(Prev);

	if (Result == -1)
	{
		return false;
	}

	m_Str.replace(Result, Prev.size(), _Next);

	return true;
}

bool GameString::SGAMEPATHDATA::IsExt(const GameString& _Ext)
{
	GameString Str = ArrExt;
	return Str == _Ext;
}


GameString GameString::GetCutText(const size_t& _Start, const size_t& _End)
{
	size_t TextCutSize = _End - _Start + 1;

	std::wstring Cut;
	Cut.resize(TextCutSize);
	wmemcpy_s(&Cut[0], TextCutSize, &m_Str[_Start], TextCutSize);

	//Cut�̶�� wstring�� ������������ ���κб��� memcpy�� ���� ������.
	return Cut;
}

size_t GameString::Find(const wchar_t* _Find)
{
	//�� m_Str���� find�� ������ ��ġ�ϴ� ù��°�� ã�� �ش� �ε����� ������
	//find first of�� �ѱ��ڶ� ������ ������ �����ϴ� �Ͱ� ������
	return m_Str.find(_Find);
}

std::vector<size_t> GameString::FindToCount(const wchar_t* _Find)
{
	size_t Check = 0;

	GameString Text = _Find;
	size_t Count = Text.Size();

	std::vector<size_t> SizeArr;

	while (true)
	{
		Check = m_Str.find(_Find, Check);

		if (std::wstring::npos == Check)
		{
			break;
		}

		SizeArr.push_back(Check);
		Check += Count;

	}

	return SizeArr;
}