#pragma once
#include <GameScene.h>

class GameCam;
class GameTile;
class GameMiniMap;
class MapMenuDlg;
class ToolMaker : public GameScene::SceneComponent
{
	friend MapMenuDlg;

private:
	SPtr<GameCam> m_Cam;
	SPtr<GameCam> UICam;
	static SPtr<GameTile>		m_TileRender;
	static SPtr<GameMiniMap>	m_MinimapRender;
	float	m_Timer;
	int		m_Way;

public:
	void Loading() override;
	void Update() override;
	void DebugRender() override;

public:
	ToolMaker();
	~ToolMaker();
};

