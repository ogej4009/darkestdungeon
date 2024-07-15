#pragma once
#include <DirectXCollision.h>
#include <DirectXCollision.inl>
#include <DirectXPackedVector.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <GameWin.h>
#include <map>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

class GameDevice : public GameRef
{
private:
	static std::map<GameWin*, SPtr<GameDevice>> m_DeviceMap;
	static GameDevice* m_HMainDevice;
	static ID3D11Device* m_MainDevice;
	static ID3D11DeviceContext* m_MainContext;

private:
	GameWin* m_ParentWin;

	// DirectX::
	// 그래픽카드의 메모리와 데이터관리를 위해서 제공하는 거야
	// 니가 그래픽카드에 새로운 텍스처나 행렬 매쉬를 로드하려면
	// 이걸 통해서 하렴
	// 컴방식의 dxguid

	UINT m_MC;
	UINT m_MQ;
	CVector m_ClearColor;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	// #include <d3d11_4.h>
	IDXGISwapChain* m_pSwapChain;

	// 랜더타겟뷰
	ID3D11Texture2D* m_pBackBufferTex;
	ID3D11Texture2D* m_pDepthStencilTex;

	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;

	D3D11_VIEWPORT m_VP;

public:
	GameDevice(GameWin* _Win);
	~GameDevice();


public:
	//Device 클래스를 포인터로 전달해줌
	static GameDevice* HMainDevice() {
		return m_HMainDevice;
	}
	//Dx용 디바이스를 전달해줌
	static ID3D11Device* MainDevice() {
		return m_MainDevice;
	}
	static ID3D11DeviceContext* MainContext() {
		return m_MainContext;
	}

	static void CreateDevice(SPtr<GameWin> _Win);

	void RenderBegin(); //그리기 시작하기 전에 깨끗이 지워서 그릴 준비.
	//이 두 함수 사이에 랜더링을 해야합니다.
	void RenderEnd();
	//기본 뷰포트 세팅
	void DEFVIEWPORTSET();
	//기본 렌더타겟뷰 세팅
	void DEFRENDERTARGETSET();

private:
	bool CreateSC(); // 스왑체인 만들고
	bool CreateDS(); // 깊이 버퍼 만들고
	bool CreateVP(); // 뷰포트

	bool m_DefInit; //기본 Init이 실행되어야 하니?
	void DefInit(); //실행되는 Init
	void MeshInit();  // 위 DefInit이 실행할 함수
	void ShaderInit(); // 쉐이더 Init
	void SmpInit(); //샘플러 Init
	void BlendInit(); //블렌더 Init
	void FontInit(); //폰트 초기화
	void RenderPathInit(); //랜더패스 Init
	void RasterizerInit();
	void DepthStencilInit();
};

