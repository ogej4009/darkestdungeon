#pragma once
#include <string>
#include <atlstr.h>
#include <vector>
// SPINE
class GameFile;
class GameString
{
public:
	std::wstring m_Str;

	class SGAMEPATHDATA
	{
	public:
		wchar_t ArrDiv[256];
		wchar_t ArrDir[256];
		wchar_t ArrFile[256];
		wchar_t ArrExt[256];
		wchar_t ArrFullFile[256];

	public:
		bool IsExt(const GameString& _Ext);
	};


public:
	GameString();
	GameString(const wchar_t* _Str);
	GameString(const char* _Str);
	GameString(const std::wstring& _Str);
	GameString(const std::string& _Str);
	~GameString();


public:
	void reserve(size_t _Size)
	{
		m_Str.reserve(_Size);
	}

public:
	SGAMEPATHDATA  PathData() const
	{
		SGAMEPATHDATA  Data;
		//경로를 입력받으면 쪼개주는 함수 _wsplitpath_s
		//이너클래스는 내부에서 외부 클래스의 멤버 m_Str에 접근할 수 있다.
		_wsplitpath_s(m_Str.c_str(), Data.ArrDiv, Data.ArrDir, Data.ArrFile, Data.ArrExt);
		swprintf_s(Data.ArrFullFile, L"%s%s", Data.ArrFile, Data.ArrExt);
		return Data;
	}

	operator std::wstring()  const
	{
		return m_Str;
	}

	operator std::string() const
	{
		return CW2A(m_Str.c_str()).m_psz;
	}

	operator const wchar_t*() const
	{
		return m_Str.c_str();
	}

	bool operator>(const  GameString& _Other)  const { return m_Str > _Other.m_Str; }
	bool operator<(const  GameString& _Other) const { return m_Str < _Other.m_Str; }
	bool operator>=(const GameString& _Other) const { return m_Str >= _Other.m_Str; }
	bool operator<=(const GameString& _Other) const { return m_Str <= _Other.m_Str; }
	bool operator==(const GameString& _Other) const { return m_Str == _Other.m_Str; }
	bool operator!=(const GameString& _Other) const { return m_Str != _Other.m_Str; }

	bool operator>(const  wchar_t* _Other) const { return m_Str > _Other; }
	bool operator<(const  wchar_t* _Other) const { return m_Str < _Other; }
	bool operator>=(const wchar_t* _Other) const { return m_Str >= _Other; }
	bool operator<=(const wchar_t* _Other) const { return m_Str <= _Other; }
	bool operator==(const wchar_t* _Other) const { return m_Str == _Other; }
	bool operator!=(const wchar_t* _Other) const { return m_Str != _Other; }

	bool operator>(const  std::wstring _Other)  const { return m_Str > _Other; }
	bool operator<(const  std::wstring _Other)  const { return m_Str < _Other; }
	bool operator>=(const std::wstring _Other)  const { return m_Str >= _Other; }
	bool operator<=(const std::wstring _Other)  const { return m_Str <= _Other; }
	bool operator==(const std::wstring _Other)  const { return m_Str == _Other; }
	bool operator!=(const std::wstring _Other)  const { return m_Str != _Other; }

	GameString operator+(const GameString& _Other)
	{
		GameString Result = m_Str + _Other.m_Str;
		return Result;
	}

	GameString& operator+=(const GameString& _Other)
	{
		m_Str += _Other.m_Str;
		return *this;
	}

	GameString& operator=(const GameString& _Other)
	{
		m_Str = _Other.m_Str;
		return *this;
	}

	const wchar_t& operator [](size_t _Count) const
	{
		return m_Str[_Count];
	}

	const wchar_t* StrPtr() const
	{
		return m_Str.c_str();
	}

	size_t Size() const
	{
		return m_Str.size();
	}

	void ReSize(const size_t _Size)
	{
		m_Str.resize(_Size);
	}


	void Change(const wchar_t* _Prev, const wchar_t* _Next);
	void ChangeParent(const wchar_t* _Prev, const wchar_t* _Next);
	bool ChangeToFirstOne(const wchar_t* _Prev, const wchar_t* _Next);

	GameString GetCutText(const size_t& _Start, const size_t& _End);
	bool IsContains(const wchar_t* _Find);
	size_t Find(const wchar_t* _Find);
	std::vector<size_t> FindToCount(const wchar_t* _Find);

	//Next는 바꿔진 결과, ...매개변수는 바꾸고 싶은 내용들
	template<typename ...Rest>
	void ChangeToAll(const wchar_t* _Next, Rest ... Arg)
	{
		ChangePush(_Next, Arg...);
	}

	//void Create(const wchar_t* _Format, ...);

private:
	template<typename ...Rest>
	void ChangePush(const wchar_t* _Next, const wchar_t* _Prev, Rest ... Arg)
	{
		//Change함수는 _Prev자리의 것을 _Next로 바꾼다.
		Change(_Prev, _Next);
		ChangePush(_Next, Arg...);
	}

	void ChangePush(const wchar_t* _Next)
	{

	}

};

