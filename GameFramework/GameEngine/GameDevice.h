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
	// �׷���ī���� �޸𸮿� �����Ͱ����� ���ؼ� �����ϴ� �ž�
	// �ϰ� �׷���ī�忡 ���ο� �ؽ�ó�� ��� �Ž��� �ε��Ϸ���
	// �̰� ���ؼ� �Ϸ�
	// �Ĺ���� dxguid

	UINT m_MC;
	UINT m_MQ;
	CVector m_ClearColor;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	// #include <d3d11_4.h>
	IDXGISwapChain* m_pSwapChain;

	// ����Ÿ�ٺ�
	ID3D11Texture2D* m_pBackBufferTex;
	ID3D11Texture2D* m_pDepthStencilTex;

	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;

	D3D11_VIEWPORT m_VP;

public:
	GameDevice(GameWin* _Win);
	~GameDevice();


public:
	//Device Ŭ������ �����ͷ� ��������
	static GameDevice* HMainDevice() {
		return m_HMainDevice;
	}
	//Dx�� ����̽��� ��������
	static ID3D11Device* MainDevice() {
		return m_MainDevice;
	}
	static ID3D11DeviceContext* MainContext() {
		return m_MainContext;
	}

	static void CreateDevice(SPtr<GameWin> _Win);

	void RenderBegin(); //�׸��� �����ϱ� ���� ������ ������ �׸� �غ�.
	//�� �� �Լ� ���̿� �������� �ؾ��մϴ�.
	void RenderEnd();
	//�⺻ ����Ʈ ����
	void DEFVIEWPORTSET();
	//�⺻ ����Ÿ�ٺ� ����
	void DEFRENDERTARGETSET();

private:
	bool CreateSC(); // ����ü�� �����
	bool CreateDS(); // ���� ���� �����
	bool CreateVP(); // ����Ʈ

	bool m_DefInit; //�⺻ Init�� ����Ǿ�� �ϴ�?
	void DefInit(); //����Ǵ� Init
	void MeshInit();  // �� DefInit�� ������ �Լ�
	void ShaderInit(); // ���̴� Init
	void SmpInit(); //���÷� Init
	void BlendInit(); //���� Init
	void FontInit(); //��Ʈ �ʱ�ȭ
	void RenderPathInit(); //�����н� Init
	void RasterizerInit();
	void DepthStencilInit();
};

