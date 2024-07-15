#pragma once
#include <GameRef.h>
#include <map>
#include <list>

// GAMESCENE 클래스가 너무 커져서 나눔
// Cam과 Renderer를 가지고 있으면서 실제 랜더링을 돌려줌

class GameRenderer;
class GameCam;
class GameScene;
class GameRenderMgr final
{
	friend GameRenderer;
	friend GameCam;
	friend GameScene;

private:
	//부모 씬 가지고 있기
	GameScene* m_pScene;
	//메인 캠 가지고 있음, Debug랜더처럼 부모 액터가 없는 랜더링을 위해서
	//따로 캠을 넣지 않으면 첫번째 캠이 메인캠이 되고 그걸 디폴트인자로 넣어줌
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

