#pragma once
#include "GameRenderer.h"

class GameFont;
class GameDebugEn
{
	friend class GameDevice;
	friend class GameRenderMgr;

private:
	class DEBUGDRAWINFO
	{
	public:
		GameCam* m_Cam;
		GameTransform* m_Trans;
		CVector Color;
		GameRenderData* RD;
	};

	class DEBUGFONTINFO
	{
	public:
		GameString _Text;
		float _Size;
		CVector _Pos;
		CVector _Color;
	};

	static size_t m_DrawCount;
	static size_t m_DrawFontCount;
	static std::vector<DEBUGDRAWINFO> m_Info;
	static std::vector<DEBUGFONTINFO> m_FontInfo;
	static std::list<GameString>::iterator m_DebugFontIter;

	static CTransData m_DebugTransData;
	static CVector m_DebugColor;
	static SPtr<GameRenderData> RectRenderData;

	static SPtr<GameFont> m_Font;
	//외부에서 생성할 수 없다.
private:
	GameDebugEn();
	~GameDebugEn();

public:
	static void Draw2DRect(GameTransform* _Trans, CVector _Color = CVector::RED);
	static void Draw2DRect(GameCam* _Cam, GameTransform* _Trans, CVector _Color = CVector::RED);
	static void Draw2DFont(const GameString& _Text, const float& _Size, const CVector& _Pos, const CVector& _Color);

private:
	static void Init();
	static void DebugRender();
};

