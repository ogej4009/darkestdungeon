#pragma once
#include <iostream>
#include <vector>
#include "GamePath.h"
#include "GameTFunc.h"


class GameFile : public GamePath
{
private:
	std::wstring m_FileName;
	std::wstring m_Ext;
	std::wstring m_OpenMode;

	size_t m_Size;
	FILE* m_pFile;

public:
	GameFile();
	GameFile(const GameFile& _Other);
	GameFile(const wchar_t* _Path, const wchar_t* _Mode);
	~GameFile();

public:
	template<typename T>
	void Write(const T& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsWrite())
		{
			assert(false);
		}
		fwrite(&_Data, sizeof(T), 1, m_pFile);
	}

	//template 특수화
	template<>
	void Write(const std::wstring& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsWrite())
		{
			assert(false);
		}

		int Size = (int)_Data.size();
		fwrite(&Size, sizeof(int), 1, m_pFile);
		fwrite(_Data.c_str(), sizeof(wchar_t) * Size, 1, m_pFile);
	}

	template<>
	void Write(const GameString& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsWrite())
		{
			assert(false);
		}

		// int를 하나 저장해야 한다.

		int Size = (int)_Data.Size();
		fwrite(&Size, sizeof(int), 1, m_pFile);
		fwrite(_Data.StrPtr(), sizeof(wchar_t) * Size, 1, m_pFile);
	}

	void Read(void* _Buffer, size_t _BufferSize)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsRead())
		{
			assert(false);
		}

		fread_s(_Buffer, _BufferSize, _BufferSize, 1, m_pFile);
	}


	template<typename T>
	void Read(T& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsRead())
		{
			assert(false);
		}
		fread_s(&_Data, sizeof(T), sizeof(T), 1, m_pFile);
	}




	template<>
	void Read(std::wstring& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsRead())
		{
			assert(false);
		}

		int Size;
		fread_s(&Size, sizeof(int), sizeof(int), 1, m_pFile);

		wchar_t* Arr = new wchar_t[Size + 1]{ 0, };

		fread_s(Arr, sizeof(wchar_t)* Size, sizeof(wchar_t) * Size, 1, m_pFile);
		_Data = Arr;

		if (nullptr != Arr)
		{
			delete[] Arr;
			Arr = nullptr;
		}
	}

	template<>
	void Read(GameString& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsRead())
		{
			assert(false);
		}

		int Size;
		fread_s(&Size, sizeof(int), sizeof(int), 1, m_pFile);

		// 받은 버퍼를 동적으로 만든다.
		wchar_t* Arr = new wchar_t[Size + 1]{ 0, };
		fread_s(Arr, sizeof(wchar_t) * Size, sizeof(wchar_t) * Size, 1, m_pFile);
		_Data = Arr;

		if (nullptr != Arr)
		{
			delete[] Arr;
			Arr = nullptr;
		}
	}

	//파일의 내용을 읽어서 txt형태로 불러오기, 반대는 바이너리
	GameString GetFileToText()
	{
		//사이즈 알아내기
		fseek(m_pFile, 0, SEEK_END);
		size_t size = ftell(m_pFile);

		//파일 포인터 처음으로 이동
		fseek(m_pFile, 0, SEEK_SET);

		char* NewText = new char[size + 1];
		memset(NewText, 0, size);
		//파일 내용을 전부 heap 영역으로 옮겨와
		//아마 스택오버플로를 대비한 건가?
		Read(NewText, size + 1);
		
		//지역변수로 옮겨와
		GameString Data = NewText;
		delete NewText;
		//값 복사해주고 끝내.
		return Data;
	}



public:
	bool IsOpen()
	{
		return nullptr != m_pFile;
	}

	bool IsWrite()
	{
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == m_OpenMode.find_first_of(L'w'))
		{
			return false;
		}

		return true;
	}

	bool IsRead()
	{
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == m_OpenMode.find_first_of(L'r'))
		{
			return false;
		}

		return true;

	}

	bool IsExist()
	{
		return GamePath::IsExist(m_Path);
	}

	int CalcSize()
	{
		int temp = fseek(m_pFile, 0, SEEK_END);
		m_Size = ftell(m_pFile);
		return (int)m_Size;
	}

private:
	void Copy(const GameFile& _Other);

public:
	GameFile& operator=(const GameFile& _Other)
	{
		Close();
		Copy(_Other);
	}

public:
	void ChangeExt(const GameString& _TargetExt, const GameString& _ChangeExt);
	void AddExt(const GameString& _TargetExt, const GameString& _AddExt);
	void PathSetting(const GameString& _Path);

	bool Open(const wchar_t* _Path, const wchar_t* _Mode);
	bool Open();

	bool Close();

	inline const std::wstring& Ext() const
	{
		return m_Ext;
	}

	inline const std::wstring& CFileName() const
	{
		return m_FileName;
	}

};

