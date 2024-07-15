#pragma once
#include <string>
#include <assert.h>
#include <Windows.h>
#include "GameString.h"


class GamePath
{
public:
	static bool IsExist(const wchar_t* _Path)
	{
		if (0 != _waccess_s(_Path, 0))
		{
			return false;
		}
		return true;
	}

	static bool IsDir(const wchar_t* _Path)
	{
		int Result = GetFileAttributesW(_Path);

		if (-1 == Result)
		{
			return false;
		}

		if (0 == (FILE_ATTRIBUTE_DIRECTORY & Result))
		{
			return false;
		}

		return true;
	}


protected:
	GameString m_Path;

public:
	const GameString& CFullPath() const
	{
		return m_Path;
	}

	std::wstring FullPath() const
	{
		return m_Path;
	}

public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

public:
	GamePath();
	~GamePath();
};

