#pragma once
#include <GameRef.h>
#include <map>
#include <list>

class GameScene;
class GameCol;
class GameColMgr
{
public:
	friend GameCol;
	friend GameScene;

private:
	std::map<unsigned int, std::list<unsigned int>> m_AllLink;
	std::map<unsigned int, std::list<SPtr<GameCol>>>::iterator SrcIter;
	std::map<unsigned int, std::list<SPtr<GameCol>>>::iterator DestIter;
	std::map<unsigned int, std::list<SPtr<GameCol>>> m_AllCol;

	std::list<SPtr<GameCol>>::iterator CStartIter;
	std::list<SPtr<GameCol>>::iterator CEndIter;

public:
	GameColMgr();
	~GameColMgr();

public:
	void Link(unsigned int _Src, unsigned int _Dest);

	//Col관련 친구들만 friend걸어서 접근하고 외부에는 Link만 공개한다.
private:
	void OtherCol(std::list<SPtr<GameCol>>& _Left, std::list<SPtr<GameCol>>& _Right);
	void PushCol(GameCol* _Col);
	void Collision();
	void Release();
};

