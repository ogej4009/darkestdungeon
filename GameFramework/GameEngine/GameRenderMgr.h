#pragma once
#include <GameRef.h>
#include <map>
#include <list>

// GAMESCENE Ŭ������ �ʹ� Ŀ���� ����
// Cam�� Renderer�� ������ �����鼭 ���� �������� ������

class GameRenderer;
class GameCam;
class GameScene;
class GameRenderMgr final
{
	friend GameRenderer;
	friend GameCam;
	friend GameScene;

private:
	//�θ� �� ������ �ֱ�
	GameScene* m_pScene;
	//���� ķ ������ ����, Debug����ó�� �θ� ���Ͱ� ���� �������� ���ؼ�
	//���� ķ�� ���� ������ ù��° ķ�� ����ķ�� �ǰ� �װ� ����Ʈ���ڷ� �־���
	GameCam* m_MainCam;
	std::map<unsigned int, SPtr<GameCam>> m_AllCam;
	std::map<unsigned int, std::list<SPtr<GameRenderer>>> m_AllRender;
	std::list<SPtr<GameRenderer>>::iterator RStartIter;
	std::list<SPtr<GameRenderer>>::iterator REndIter;


public:
	std::map<unsigned int, std::list<SPtr<GameRenderer>>>::iterator GroupIter;

public:
	GameRenderMgr(GameScene* _pScene);
	~GameRenderMgr();

private:
	void PushCam(GameCam* _Cam);
	void PushRender(GameRenderer* _Render);
	void Render();
	void Release();


public:
	GameCam* MainCam()
	{
		return m_MainCam;
	}

};

