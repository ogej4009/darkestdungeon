#pragma once
#include "GameRenderer.h"
#include "GameSprite.h"

//���÷� ���������� ����� ������� ������ ���ִ� Ŭ����
//���� ���⿡ ��ο� ������ �̹��� �̸��� �־��ָ� �ȴ�.
class GameImage : public GameRenderer
{
private:
	SPtr<GameSprite> m_Sprite;
	size_t m_Index;
	GameRenderData* RD;
	CVector m_Color;

public:
	GameImage();
	~GameImage();

public:
	CVector Color()
	{
		return m_Color;
	}

	void Color(CVector _Color)
	{
		m_Color = _Color;
	}

	size_t Index()
	{
		return m_Index;
	}

	void Index(size_t _Index);

	SPtr<GameSprite> Sprite()
	{
		return m_Sprite;
	}

	void Sprite(const GameString& _Name, size_t _Index = 0);
	void Sprite(const SPtr<GameSprite> _Sprite);
	void SpriteUv(const GameString& _Name, const std::vector<CVector>& _UvData);
	//void ChangeMesh(const GameString& _MeshName);

public:
	void Init(unsigned int _Order = 0) override;
	void RenderUpdate(SPtr<GameCam> _Cam) override;
};

