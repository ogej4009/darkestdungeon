#pragma once
#include "GameFile.h" 
#include <vector>


class GameDic : public GamePath
{
public:
	GameDic();
	~GameDic();

public:
	void PathSetting(const GameString& _Path);

	GameFile CreateGameFile(const GameString& _FileName);
	GameFile CreateGameFileReadOnly(const GameString& _FileName);
	GameString CreateGameFilePath(const GameString& _FileName);

	bool Move(const GameString& _Name);
	void MoveParent();
	void MoveParent(const GameString& _Name);
	bool IsFile(const GameString& _FileName);

	std::vector<GameFile> DirToAllFile();
	std::vector<GameFile> DirToAllDir();
	std::vector<GameFile> DirToAllFile(const GameString& _Delimiter);

	bool TryFindFileRecursive(const GameString& _Name);
};

