#include "GameSprite.h"

GameSprite::GameSprite()
{

}

GameSprite::~GameSprite()
{

}

bool GameSprite::CreateSprite(const GameString& _TexName, size_t _W, size_t _H)
{
	m_Tex = GameTexture::Find(_TexName);

	if (nullptr == m_Tex)
	{
		ASSERTMSG;
	}

	m_CutPos.resize(_W * _H);

	//Y값으로 먼저 세팅하고, X값으로 각 벡터마다 세팅
	m_CutPos2D.resize(_H);
	for (size_t i = 0; i < m_CutPos2D.size(); i++)
	{
		m_CutPos2D[i].resize(_W);
	}

	CVector StartPos = CVector::ZERO;

	//Uv원소값을 의미한다.
	StartPos.z = 1.0f / _W;
	StartPos.w = 1.0f / _H;

	// 샘플러 스테이트가 여기에 영향을 줄수 있다.
	// 실수 오차 때문에. 그때는 약간 보정을 해주거나 보간을 해주면 된다.
	// wrap모드에서는 건너편것이 나올수가 있다.
	// clamp 0~1사이가 아니라면 안나와.
	// mirror 반대로 뒤집어져서 나왔다가 다시 또 뒤집히는 식으로 나온다.
	// 라이너 포인트

	for (size_t Y = 0; Y < _H; Y++)
	{
		for (size_t X = 0; X < _W; X++)
		{
			StartPos.x = StartPos.z * X;
			StartPos.y = StartPos.w * Y;
			size_t Index = Y * _W + X;
			m_CutPos[Index] = StartPos;
			m_CutPos2D[Y][X] = StartPos;
		}
	}
	return true;
}