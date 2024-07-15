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

	//Y������ ���� �����ϰ�, X������ �� ���͸��� ����
	m_CutPos2D.resize(_H);
	for (size_t i = 0; i < m_CutPos2D.size(); i++)
	{
		m_CutPos2D[i].resize(_W);
	}

	CVector StartPos = CVector::ZERO;

	//Uv���Ұ��� �ǹ��Ѵ�.
	StartPos.z = 1.0f / _W;
	StartPos.w = 1.0f / _H;

	// ���÷� ������Ʈ�� ���⿡ ������ �ټ� �ִ�.
	// �Ǽ� ���� ������. �׶��� �ణ ������ ���ְų� ������ ���ָ� �ȴ�.
	// wrap��忡���� �ǳ������� ���ü��� �ִ�.
	// clamp 0~1���̰� �ƴ϶�� �ȳ���.
	// mirror �ݴ�� ���������� ���Դٰ� �ٽ� �� �������� ������ ���´�.
	// ���̳� ����Ʈ

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