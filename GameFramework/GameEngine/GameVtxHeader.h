#pragma once
#include <GameMath.h>

class Vertex2d
{
public:
	CVector m_Pos; // 사실 SVEC3여도 문제가 없다.
	CVector m_UV;	// 사실 m_UV애는 SVEC2
	CVector m_Color;// 사실 m_Color애는 SVEC2
};
