#pragma once
#include "GameDevice.h"
#include "GameShader.h"
#include <GameRes.h>
#include <DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

class GameTexture : public GameRes<GameTexture>
{
private:
	DirectX::ScratchImage m_Image;
	size_t m_W;
	size_t m_H;

	// 리소스로 분류된다.
	ID3D11Resource* m_pTex;
	ID3D11RenderTargetView*		  m_pRTV;
	ID3D11DepthStencilView*		  m_pDSV;

	// 이게 쉐이더에 넣어주는방법
	ID3D11ShaderResourceView*		  m_pSRV;

	ID3D11RenderTargetView*		  RTV() { return m_pRTV; }
	ID3D11DepthStencilView*		  DSV() { return m_pDSV; }
	ID3D11ShaderResourceView*	  SRV() { return m_pSRV; }


public:
	GameTexture();
	~GameTexture();

public:
	static SPtr<GameTexture> Load(const GameString& _Path)
	{
		GameString Name = _Path.PathData().ArrFullFile;
		GameTexture* NewRes = CreateRes(Name);
		NewRes->m_Path = _Path;
		if (false == NewRes->Load()) { ASSERTMSG; }
		return NewRes;
	}

	bool Load();
	void SettingTex(GameShader::SHADER_TYPE _Type, int _Index);

public:
	void VSUpdate(int _Index);
	void HSUpdate(int _Index);
	void DSUpdate(int _Index);
	void GSUpdate(int _Index);
	void PSUpdate(int _Index);

};

