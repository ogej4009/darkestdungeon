#pragma once
#include <GameRes.h>

class GameShader;
class GameVtxShader;
class GamePixShader;
class GameRenderer;
class GameBlend;
class GameRasterizer;
class GameDepthStencil;
class GameRenderMaker : public GameRes<GameRenderMaker>
{
private:

	//������Ÿ�� ���� ��뿡�� �������� üũ�뵵
	bool m_IsClone;
	//<������ ���� ������>
	SPtr<GameVtxShader> m_VTXShader;
	SPtr<GamePixShader> m_PIXShader;
	SPtr<GameBlend> m_Blend;
	SPtr<GameRasterizer> m_Rasterizer;
	SPtr<GameDepthStencil> m_DepthStencil;
	std::vector<GameShader*> m_Shader;

	static GameShader* LastShader[5];
	static GameBlend* LastBlend;
	static GameRasterizer* LastRasterizer;
	static GameDepthStencil* LastDepthStencil;

public:
	GameRenderMaker();
	GameRenderMaker(const GameRenderMaker& _Other);
	~GameRenderMaker();

public:
	//���Լ�
	GameShader* SHADER(size_t _Index)
	{
		return m_Shader[_Index];
	}

	GameVtxShader* VTXSHADER()
	{
		return m_VTXShader;
	}

	GamePixShader* PIXSHADER()
	{
		return m_PIXShader;
	}

	//�̸������� ���� ������ ã�� �� �ְ� �ϱ�
	void VTXShader(const GameString& _Name);
	void PIXShader(const GameString& _Name);
	void Blend(const GameString& _Name);
	void Rasterizer(const GameString& _Name);
	void DepthStencil(const GameString& _Name);

	bool IsClone()
	{
		return m_IsClone;
	}

	SPtr<GameRenderMaker> Clone();

	static SPtr<GameRenderMaker> Create(const GameString& _Name)
	{
		GameRenderMaker* NewRes = CreateRes(_Name);
		NewRes->Create();
		return NewRes;
	}

	bool Create();
	void Setting();

	//RD������ ���� ������ ���� ����ִ°� ���½����ִ� �Լ�
	static void LastReset();
};

