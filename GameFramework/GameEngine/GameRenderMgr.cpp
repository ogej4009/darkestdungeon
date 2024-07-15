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

	//�������� ����, RTV�� �ʱ�ȭ��
	GameDevice::HMainDevice()->RenderBegin();
	GameDevice::HMainDevice()->DEFRENDERTARGETSET();
	GameDevice::HMainDevice()->DEFVIEWPORTSET();

	//��� ī�޶� ���鼭 ī�޶� ������
	for (std::pair<const unsigned int, SPtr<GameCam>>& Item : m_AllCam)
	{
		//�� ī�޶��� ������ �׷�(unsigned int)�� ������
		for (unsigned int Value : Item.second->m_Group)
		{
			//��� �������� uint��(������ �׷�)���� ����� ������ ������
			GroupIter = m_AllRender.find(Value);

			if (GroupIter == m_AllRender.end())
			{
				continue;
			}

			//�ش� �׷��� �����Ѵٸ� �����ͼ� ������ Rendering��
			for (auto& Renderer : GroupIter->second)
			{
				//�װų� off���¶�� �Ѿ���ǳ�
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
	//�������� ��, ����ü�� �ʱ�ȭ��
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
		// ���ٸ� �����.
		m_AllRender.insert(std::map<unsigned int, std::list<SPtr<GameRenderer>>>::value_type(_Render->m_Order, std::list<SPtr<GameRenderer>>()));
		FindIter = m_AllRender.find(_Render->m_Order);
	}
	FindIter->second.push_back(_Render);
}

//�ѹ��� ���鼭 ���� ģ������ ���ְŶ�.
//�� ����� ���� �Ѱ� �ؼ� �� 2������ �ٵ�� �־ ���⻩�ذɷθ�
//���������� �ʴ´�.
void GameRenderMgr::Release()
{
	//�Ϲ��� for���̳� foreach������ ���°��
	//���� ��������� �ε����� �и��� ������ �߻��Ѵ�.
	//���� ���ͷ����͸� Ȱ���ؼ� ������

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

			//���� ���¶�� �����ش�. erase�� ������ ���� ������
			//������ �ֹǷ� �װ� �޾Ƽ� ��� �����ϸ� �ȴ�.
			RStartIter = render.second.erase(RStartIter);
		}
	}
}
