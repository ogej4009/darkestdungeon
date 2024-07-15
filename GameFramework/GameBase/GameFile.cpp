#include "GameFile.h"
#include <atlstr.h>
#include <wchar.h>
#include "GameMacro.h"


GameFile::GameFile(const wchar_t* _Path, const wchar_t* _Mode) : m_pFile(nullptr), m_Size(0)
{
	Open(_Path, _Mode);
}

GameFile::GameFile(const GameFile& _Other) : m_pFile(nullptr), m_Size(0)
{
	Copy(_Other);
}

GameFile::GameFile() : m_pFile(nullptr), m_Size(0)
{
}


GameFile::~GameFile()
{
	Close();
}

void GameFile::PathSetting(const GameString& _Path)
{
	m_Ext = _Path.PathData().ArrExt;
	m_FileName = _Path.PathData().ArrFile;
	m_Path = _Path;
}

void GameFile::ChangeExt(const GameString& _TargetExt, const GameString& _ChangeExt)
{
	GameString temp = m_Path;

	if (L"" == m_Ext)
	{
		ASSERTMSG;
	}

	if (_TargetExt == m_Ext)
	{
		size_t index = m_Path.m_Str.find_last_of(L".");
		m_Path.m_Str.erase(index, _TargetExt.Size());
		m_Path.m_Str = m_Path.m_Str + _ChangeExt.m_Str;

		_wrename(temp.m_Str.c_str(), m_Path.m_Str.c_str());
		PathSetting(m_Path.m_Str.c_str());
	}
}

void GameFile::AddExt(const GameString& _TargetExt, const GameString& _AddExt)
{
	GameString temp = m_Path;

	if (L"" == m_Ext)
	{
		ASSERTMSG;
	}

	if (_TargetExt == m_Ext)
	{
		m_Path.m_Str = m_Path.m_Str + _AddExt.m_Str;
		_wrename(temp.m_Str.c_str(), m_Path.m_Str.c_str());
		PathSetting(m_Path.m_Str.c_str());
	}
}

bool GameFile::Open()
{
	return Open(m_Path, m_OpenMode.c_str());
}

bool GameFile::Open(const wchar_t* _Path, const wchar_t* _Mode)
{
	PathSetting(_Path);
	m_OpenMode = _Mode;
	_wfopen_s(&m_pFile, m_Path, m_OpenMode.c_str());

	if (nullptr == m_pFile)
	{
		return false;
	}

	return true;
}

bool GameFile::Close()
{
	int result = 0;
	if (nullptr != m_pFile)
	{
		result = fclose(m_pFile);
		m_pFile = nullptr;
	}

	if (0 != result)
	{
		return false;
	}

	return true;
}

void GameFile::Copy(const GameFile& _Other)
{
	PathSetting(_Other.m_Path);
	m_pFile = _Other.m_pFile;
	m_OpenMode = _Other.m_OpenMode;
}