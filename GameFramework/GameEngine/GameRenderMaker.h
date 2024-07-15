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

	//프로토타입 패턴 사용에서 복제여부 체크용도
	bool m_IsClone;
	//<렌더링 관련 정보들>
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
	//겟함수
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

	//이름가지고 내부 정보들 찾을 수 있게 하기
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

	//RD정보중 가장 마지막 정보 들고있는거 리셋시켜주는 함수
	static void LastReset();
};

