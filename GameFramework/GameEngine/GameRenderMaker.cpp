#include "GameRenderMaker.h"
#include "GameVtxShader.h"
#include "GamePixShader.h"
#include "GameBlend.h"
#include "GameShader.h"
#include "GameRasterizer.h"
#include "GameDepthStencil.h"

 GameShader*			GameRenderMaker::LastShader[5];
 GameBlend*			GameRenderMaker::LastBlend;
 GameRasterizer*		GameRenderMaker::LastRasterizer;
 GameDepthStencil*		GameRenderMaker::LastDepthStencil;


GameRenderMaker::GameRenderMaker() : m_IsClone(false)
{
	m_Shader.resize((size_t)GameShader::SHADER_TYPE::ST_MAX);
}

GameRenderMaker::GameRenderMaker(const GameRenderMaker& _Other)
	: m_VTXShader(_Other.m_VTXShader),
	m_PIXShader(_Other.m_PIXShader),
	m_Blend(_Other.m_Blend),
	m_Rasterizer(_Other.m_Rasterizer),
	m_IsClone(true)
{

}

GameRenderMaker::~GameRenderMaker()
{

}

void GameRenderMaker::VTXShader(const GameString& _Name)
{
	m_VTXShader = GameVtxShader::Find(_Name);

	if (nullptr == m_VTXShader)
	{
		ASSERTMSG;
	}

	m_Shader[(int)m_VTXShader->ShType()] = m_VTXShader;
}

void GameRenderMaker::PIXShader(const GameString& _Name)
{
	m_PIXShader = GamePixShader::Find(_Name);

	if (nullptr == m_PIXShader)
	{
		ASSERTMSG;
	}

	m_Shader[(int)m_PIXShader->ShType()] = m_PIXShader;
}

void GameRenderMaker::Blend(const GameString& _Name)
{
	m_Blend = GameBlend::Find(_Name);

	if (nullptr == m_Blend)
	{
		ASSERTMSG;
	}
}

void GameRenderMaker::Rasterizer(const GameString& _Name)
{
	m_Rasterizer = GameRasterizer::Find(_Name);

	if (nullptr == m_Rasterizer)
	{
		ASSERTMSG;
	}
}


void GameRenderMaker::DepthStencil(const GameString& _Name)
{
	m_DepthStencil = GameDepthStencil::Find(_Name);

	if (nullptr == m_DepthStencil)
	{
		ASSERTMSG;
	}
}


void GameRenderMaker::Setting()
{
	//RenderPath에서 내가 가진 쉐이더가 있다면
	//전부 Setting을 시켜준다.

	for (size_t i = 0; i < m_Shader.size(); i++)
	{
		//마지막 자원들을 기억하게 해서 바뀌는 경우에만
		//Setting을 실행하게 바꿨음
		if (nullptr == m_Shader[i]
			&& LastShader[i] == m_Shader[i])
		{
			continue;
		}

		m_Shader[i]->Setting();
		LastShader[i] = m_Shader[i];
	}

	if (nullptr != m_Blend
		&& LastBlend != m_Blend)
	{
		m_Blend->Setting();
		LastBlend = m_Blend;
	}

	if (nullptr != m_Rasterizer
		&& LastRasterizer != m_Rasterizer)
	{
		m_Rasterizer->Setting();
		LastRasterizer = m_Rasterizer;
	}

	if (nullptr != m_DepthStencil
		&& LastDepthStencil != m_DepthStencil)
	{
		m_DepthStencil->Setting();
		LastDepthStencil = m_DepthStencil;
	}
}


bool GameRenderMaker::Create()
{
	return true;
}

//얕은복사 형태로 복사한 후에 변경할 부분을 깊은복사 하면 된다.
SPtr<GameRenderMaker> GameRenderMaker::Clone()
{
	return new GameRenderMaker(*this);
}

//RenderMGR의 Render에서 호출됨
void GameRenderMaker::LastReset()
{
	for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; i++)
	{
		LastShader[i] = nullptr;
	}

	LastBlend = nullptr;
	LastRasterizer = nullptr;
	LastDepthStencil = nullptr;
}