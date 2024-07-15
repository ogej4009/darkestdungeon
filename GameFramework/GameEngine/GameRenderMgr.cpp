#include "GameRenderMgr.h"
#include "GameCam.h"
#include "GameRenderer.h"
#include "GameDevice.h"
#include <GameTFunc.h>
#include "GameScene.h"
#include "GameDebugEn.h"

GameRenderMgr::GameRenderMgr(GameScene* _pScene) : m_pScene(_pScene), m_MainCam(nullptr)
{
}


GameRenderMgr::~GameRenderMgr()
{
}

void GameRenderMgr::Render()
{
	GameRenderMaker::LastReset();

	//랜더링의 시작, RTV를 초기화함
	GameDevice::HMainDevice()->RenderBegin();
	GameDevice::HMainDevice()->DEFRENDERTARGETSET();
	GameDevice::HMainDevice()->DEFVIEWPORTSET();

	//모든 카메라를 돌면서 카메라를 가져옴
	for (std::pair<const unsigned int, SPtr<GameCam>>& Item : m_AllCam)
	{
		//각 카메라의 랜더링 그룹(unsigned int)을 가져옴
		for (unsigned int Value : Item.second->m_Group)
		{
			//모든 랜더에서 uint값(랜더링 그룹)으로 저장된 랜더를 가져옴
			GroupIter = m_AllRender.find(Value);

			if (GroupIter == m_AllRender.end())
			{
				continue;
			}

			//해당 그룹이 존재한다면 가져와서 각각을 Rendering함
			for (auto& Renderer : GroupIter->second)
			{
				//죽거나 off상태라면 넘어가려므나
				if (false == Renderer->IsUpdate())
				{
					continue;
				}

				Renderer->RenderUpdate(Item.second);
				Renderer->Render(Item.second);
			}
		}
	}

	m_pScene->DebugRender();
	GameDebugEn::DebugRender();
	//랜더링의 끝, 스왑체인 초기화됨
	GameDevice::HMainDevice()->RenderEnd();
}

void GameRenderMgr::PushCam(GameCam* _Cam)
{
	if (nullptr == _Cam)
	{
		ASSERTMSG;
	}

	m_AllCam.insert(std::map<unsigned int, SPtr<GameCam>>::value_type(_Cam->Order(), _Cam));

	if (nullptr == m_MainCam)
	{
		m_MainCam = _Cam;
	}
}

void GameRenderMgr::PushRender(GameRenderer* _Render)
{
	std::map<unsigned int, std::list<SPtr<GameRenderer>>>::iterator FindIter
		= m_AllRender.find(_Render->m_Order);

	if (m_AllRender.end() == FindIter)
	{
		// 없다면 만든다.
		m_AllRender.insert(std::map<unsigned int, std::list<SPtr<GameRenderer>>>::value_type(_Render->m_Order, std::list<SPtr<GameRenderer>>()));
		FindIter = m_AllRender.find(_Render->m_Order);
	}
	FindIter->second.push_back(_Render);
}

//한바퀴 돌면서 죽은 친구들은 빼주거라.
//단 여기랑 딴곳 한곳 해서 총 2곳에서 붙들고 있어서 여기빼준걸로만
//지워지지는 않는다.
void GameRenderMgr::Release()
{
	//일반적 for문이나 foreach문으로 도는경우
	//내부 삭제연산시 인덱스가 밀리는 현상이 발생한다.
	//따라서 이터레이터를 활용해서 삭제함

	for (auto& render : m_AllRender)
	{
		RStartIter = render.second.begin();
		REndIter = render.second.end();

		for (; RStartIter != REndIter;)
		{
			if (false == (*RStartIter)->IsDeath())
			{
				++RStartIter;
				continue;
			}

			//죽은 상태라면 지워준다. erase는 지워진 다음 순서를
			//리턴해 주므로 그걸 받아서 계속 진행하면 된다.
			RStartIter = render.second.erase(RStartIter);
		}
	}
}
