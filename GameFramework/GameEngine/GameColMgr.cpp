#include "GameColMgr.h"
#include "GameCol.h"


GameColMgr::GameColMgr()
{
}


GameColMgr::~GameColMgr()
{
}

void GameColMgr::Collision()
{
	for (auto& DestList : m_AllLink)
	{
		SrcIter = m_AllCol.find(DestList.first);

		if (SrcIter == m_AllCol.end())
		{
			continue;
		}

		std::list<SPtr<GameCol>>& SrcColList = SrcIter->second;

		if (0 >= SrcColList.size())
		{
			continue;
		}

		for (auto & DestOrder : DestList.second)
		{
			DestIter = m_AllCol.find(DestOrder);

			std::list<SPtr<GameCol>>& DestColList = DestIter->second;

			if (0 >= DestColList.size())
			{
				continue;
			}

			if (DestList.first == DestOrder)
			{
				int a = 0;
			}
			else
			{
				OtherCol(SrcColList, DestColList);
			}
		}
	}
}

void GameColMgr::OtherCol(std::list<SPtr<GameCol>>& _Left, std::list<SPtr<GameCol>>& _Right)
{
	for (auto& LeftCol : _Left)
	{
		for (auto& RightCol : _Right)
		{
			LeftCol->Collision(RightCol);
		}
	}
}

void GameColMgr::PushCol(GameCol* _Col)
{
	std::map<unsigned int, std::list<SPtr<GameCol>>>::iterator FindIter = m_AllCol.find(_Col->m_Order);

	if (m_AllCol.end() == FindIter)
	{
		m_AllCol.insert(std::map<unsigned int, std::list<SPtr<GameCol>>>::value_type(_Col->m_Order, std::list<SPtr<GameCol>>()));
		FindIter = m_AllCol.find(_Col->m_Order);
	}
	FindIter->second.push_back(_Col);
}

void GameColMgr::Link(unsigned int _Src, unsigned int _Dest)
{
	std::map<unsigned int, std::list<unsigned int>>::iterator FindIter = m_AllLink.find(_Src);

	if (m_AllLink.end() == FindIter)
	{
		m_AllLink.insert(std::map<unsigned int, std::list<unsigned int>>::value_type(_Src, std::list<unsigned int>()));
		FindIter = m_AllLink.find(_Src);
	}

	FindIter->second.push_back(_Dest);
}

//충돌체가 죽은 상태로 확인되면 해당 충돌체의 Release()를 실행시켜준다.
void GameColMgr::Release()
{
	for (auto& _Col : m_AllCol)
	{
		CStartIter = _Col.second.begin();
		CEndIter = _Col.second.end();

		for (; CStartIter != CEndIter;)
		{
			if (false == (*CStartIter)->IsDeath())
			{
				++CStartIter;
				continue;
			}
			(*CStartIter)->Release();
			CStartIter = _Col.second.erase(CStartIter);
		}
	}
}