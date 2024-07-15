#include "GameDic.h"
#include <atlstr.h>
#include <io.h>
#include "GameMacro.h"


GameDic::GameDic()
{
	wchar_t ArrSize[1024];

	if (0 == GetCurrentDirectoryW(1024, ArrSize))
	{
		ASSERTMSG;
	}

	GameString Path = ArrSize;
	PathSetting(Path);

}

GameDic::~GameDic()
{
}

bool GameDic::IsFile(const GameString& _FileName)
{
	GameString FindPath = m_Path;
	FindPath += L"\\";
	FindPath += _FileName;

	return GamePath::IsExist(FindPath);
}

std::vector<GameFile> GameDic::DirToAllFile()
{
	return DirToAllFile(L"*.*");
}

std::vector<GameFile> GameDic::DirToAllFile(const GameString& _Delimiter)
{
	std::vector<GameFile> ReturnList;

	std::wstring FindPath = m_Path + L"\\" + _Delimiter;
	_wfinddata_t fd;

	std::wstring Check;
	GameString temp;

	intptr_t handle = _wfindfirst(FindPath.c_str(), &fd);
	do
	{
		Check = L".";
		if (fd.name == Check)
		{
			continue;
		}
		Check = L"..";
		if (fd.name == Check)
		{
			continue;
		}

		GameFile NewFile;
		NewFile.PathSetting(m_Path + L"\\" + fd.name);
		ReturnList.push_back(NewFile);
	} while (0 == _wfindnext(handle, &fd));

	_findclose(handle); // <io.h>

	return ReturnList;
}

std::vector<GameFile> GameDic::DirToAllDir()
{
	std::vector<GameFile> ReturnList;

	std::wstring FindPath = m_Path + L"\\" + "*.*";
	_wfinddata_t fd;

	std::wstring Check;
	GameString temp;

	intptr_t handle = _wfindfirst(FindPath.c_str(), &fd);
	do
	{
		Check = L".";
		if (fd.name == Check)
		{
			continue;
		}
		Check = L"..";
		if (fd.name == Check)
		{
			continue;
		}

		GameFile NewFile;
		NewFile.PathSetting(m_Path + L"\\" + fd.name);
		if (IsDir(NewFile.CFullPath()))
		{
			ReturnList.push_back(NewFile);
		}
	} while (0 == _wfindnext(handle, &fd));

	_findclose(handle); // <io.h>

	return ReturnList;
}



bool GameDic::Move(const GameString& _Name)
{
	GameString MovePath = m_Path;
	MovePath += L"\\";
	MovePath += _Name;

	if (_Name == L"")
	{
		ASSERTMSG;
	}

	if (false == IsDir(MovePath))
	{
		IsDir(MovePath);
		ASSERTMSG;
		return false;
	}

	m_Path = MovePath;

	return true;
}

void GameDic::MoveParent()
{
	GameString FileName = L"\\";
	FileName += m_Path.PathData().ArrFile;
	m_Path.ChangeParent(FileName, L"");
}

void GameDic::MoveParent(const GameString& _Name)
{
	GameString Data = _Name;
	GameString LeftDir = m_Path.PathData().ArrFile;
	if (LeftDir != Data)
	{
		MoveParent();
		MoveParent(_Name);
	}
}

//해당 파일명을 찾고 자동으로 이동까지 해줌. 결과를 bool로 리턴함
bool GameDic::TryFindFileRecursive(const GameString& _Name)
{
	std::vector<GameFile> list = DirToAllDir();
	std::vector<GameFile>::iterator findIter = list.begin();
	int index = (int)list.size();
	bool result = false;

	while (true)
	{
		//종료조건
		if (index == 0)
		{
			break;
		}

		if (findIter->CFileName() != L"" && findIter->Ext() != L".tlog" && findIter->CFileName() != L"TP")
		{
			if (_Name == findIter->CFileName())
			{
				result = true;
				Move(_Name);
				break;
			}
			else
			{
				if (findIter->CFileName() == L"spider_webber")
				{
					int a = 0;
				}

				Move(findIter->CFileName());
				if (TryFindFileRecursive(_Name))
				{
					result = true;
					break;
				}
				MoveParent();
			}

		}

		
		++findIter;
		--index;
	}
	

	return result;
}


void GameDic::PathSetting(const GameString& _Path)
{
	if (0 != (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(_Path)))
	{
		m_Path = _Path; //디렉토리 경로
		return;
	}

	m_Path = _Path;
	MoveParent(); //파일경로를 받은 경우

}

GameFile GameDic::CreateGameFile(const GameString& _FileName)
{
	GameString CreatePath = CreateGameFilePath(_FileName);
	GameFile NewFile;
	NewFile.Open(CreatePath, L"wb");
	NewFile.Close();
	return GameFile(NewFile);
}

GameFile GameDic::CreateGameFileReadOnly(const GameString& _FileName)
{
	GameString CreatePath = CreateGameFilePath(_FileName);
	GameFile NewFile;
	NewFile.Open(CreatePath, L"rb");
	NewFile.Close();
	return GameFile(NewFile);
}


GameString GameDic::CreateGameFilePath(const GameString& _FileName)
{
	return m_Path + L"\\" + _FileName;
}
