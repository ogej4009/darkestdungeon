#include "GameImage.h"
#include "GameSampler.h"
#include "GameTexture.h"
#include "GameCam.h"

GameImage::GameImage() : m_Index(0)
{
}


GameImage::~GameImage()
{
}

void GameImage::Init(unsigned int _Order /*=0*/)
{
	GameRenderer::Init(_Order);
	m_Color = CVector::WHITE;
	RD = CreateRenderData(L"2DRECT", L"2DSPRITE");
	RD->SMP(L"PSMP", L"PSMP");
	RD->SMP(L"LSMP", L"LSMP");
	RD->CB(L"TEXCOLOR", m_Color, true);
}


void GameImage::Sprite(const GameString& _Name, size_t _Index /*= 0*/)
{
	m_Sprite = GameSprite::Find(_Name);

	if (nullptr == m_Sprite)
	{
		ASSERTMSG;
	}

	Index(_Index);
}

void GameImage::Sprite(const SPtr<GameSprite> _Sprite)
{
	m_Sprite = _Sprite;

	if (nullptr == m_Sprite)
	{
		ASSERTMSG;
	}
}


void GameImage::SpriteUv(const GameString& _Name, const std::vector<CVector>& _UvData)
{
	m_Sprite = GameSprite::Find(_Name);

	if (nullptr == m_Sprite)
	{
		ASSERTMSG;
	}

	m_Sprite->ChangeCutData(_UvData);
}

void GameImage::Index(size_t _Index)
{
	m_Index = _Index;
}

void GameImage::RenderUpdate(SPtr<GameCam> _Cam)
{
	RD->TEX("SpriteTex", m_Sprite->Tex());
	RD->CB("TEXCUT", m_Sprite->CutData(m_Index));
}
