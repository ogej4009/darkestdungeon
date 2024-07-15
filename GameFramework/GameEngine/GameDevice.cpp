#include "GameDevice.h"
#include <GameTFunc.h>
#include "GameDebugEn.h"

GameDevice* GameDevice::m_HMainDevice = nullptr;
ID3D11Device* GameDevice::m_MainDevice = nullptr;
ID3D11DeviceContext* GameDevice::m_MainContext = nullptr;

std::map<GameWin*, SPtr<GameDevice>> GameDevice::m_DeviceMap;


GameDevice::GameDevice(GameWin* _Win) 
	: m_ParentWin(_Win)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pBackBufferTex(nullptr)
	, m_pDepthStencilTex(nullptr)
	, m_DefInit(false)
{
	if (nullptr == m_ParentWin)
	{
		ASSERTMSG;
	}

	int iFlag = 0;


#ifdef _DEBUG
	// 디버그 모드로 만들어라.
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif 
	// 디바이스 레벨 디바이스의 버전을 알려주는것.
	// 안쓰겠습니다.D3D_FEATURE_LEVEL_11_1
	// 쉐이더 버전 5를 사용하기 위한것
	// 낮은 쉐이더 버전을 D3D_FEATURE_LEVEL_9
	// 쓸수는 있지만 안쓸거다.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// 초기화 컬러.
	m_ClearColor = CVector(0.0F, 0.0F, 1.0F, 1.0F);

	HRESULT Result;

	//_In_opt_ IDXGIAdapter* pAdapter, // 디바이스 만들때 너희가 특별히 짠 com용 인터페이스 
	//D3D_DRIVER_TYPE DriverType, // 나를 .
	//HMODULE Software,
	//UINT Flags,
	//_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	//UINT FeatureLevels,
	//UINT SDKVersion,
	//_COM_Outptr_opt_ ID3D11Device** ppDevice,
	//_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
	//_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext

	Result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // 소프트 웨어면 CPU로도 돌릴수 있다. 
		nullptr, // 그냥 뭔지 몰라
		iFlag, // 디버그 모드로 만들어라. 릴리즈면 0 그냥 만들어라
		nullptr, // 몰라 가능 한 최대버전
		0, // D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 넣어줘도 될꺼다.,
		D3D11_SDK_VERSION, // 현재 윈도우에 설치된 SDK버전 
		&m_pDevice,
		&eLv,
		&m_pContext);

	// 화면을 랜더링 할수 있는 준비에 필요한
	// m_pDevice // 리소스
	// m_pContext // 랜더링
	// 을 얻어온것

	if (S_OK != Result || nullptr == m_pDevice || nullptr == m_pContext)
	{
		ASSERTMSG;
	}

	m_MC = 4;
	Result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
	if (S_OK != Result)
	{
		m_MC = 1;
		Result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
		if (S_OK != Result)
		{
			ASSERTMSG;
		}
	}

	if (false == CreateSC())
	{
		ASSERTMSG;
	}

	if (false == CreateDS())
	{
		ASSERTMSG;
	}

	if (false == CreateVP())
	{
		ASSERTMSG;
	}

	if (false == m_DefInit)
	{
		m_HMainDevice = this;
		m_MainDevice = m_pDevice;
		m_MainContext = m_pContext;

		DefInit();
		m_DefInit = true;
	}

}



GameDevice::~GameDevice()
{
	if (nullptr != m_pDSV)
	{
		m_pDSV->Release();
	}
	if (nullptr != m_pRTV)
	{
		m_pRTV->Release();
	}
	if (nullptr != m_pDepthStencilTex)
	{
		m_pDepthStencilTex->Release();
	}
	if (nullptr != m_pBackBufferTex)
	{
		m_pBackBufferTex->Release();
	}
	if (nullptr != m_pSwapChain)
	{
		m_pSwapChain->Release();
	}
	if (nullptr != m_pContext)
	{
		m_pContext->Release();
	}
	if (nullptr != m_pDevice)
	{
		m_pDevice->Release();
	}
}



void GameDevice::CreateDevice(SPtr<GameWin> _Win)
{
	// 만들었는데 쓰지는 않겠습니다.
	// 그럼 알아서 해라.
	// 초기화
	GameDevice* NewDevice = new GameDevice(_Win);
	
	m_DeviceMap.insert(std::map<GameWin*, SPtr<GameDevice>>::value_type(_Win, NewDevice));
}


bool GameDevice::CreateSC()
{
	DXGI_SWAP_CHAIN_DESC SCINFO;
	memset(&SCINFO, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	SCINFO.BufferDesc.Width = m_ParentWin->Size().uix();
	SCINFO.BufferDesc.Height = m_ParentWin->Size().uiy();

	// 60 프레임 
	SCINFO.BufferDesc.RefreshRate.Numerator = 60;
	SCINFO.BufferDesc.RefreshRate.Denominator = 1;

	// HD모니터라면
	// DXGI_FORMAT_R32G32B32A32_UINT

	SCINFO.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCINFO.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SCINFO.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 화면에 출력하는 용도다.
	SCINFO.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 애는 최종 버퍼라 
	// 멀티샘플링 퀄리티를 신경쓸 필요가 없다.
	// 카운트도 1이다.
	SCINFO.SampleDesc.Quality = 0;
	SCINFO.SampleDesc.Count = 1;
	SCINFO.OutputWindow = m_ParentWin->GetHWND();

	SCINFO.BufferCount = 2;
	// SCINFO.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// 최신버전 스왑
	SCINFO.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SCINFO.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// IDXGISwapChain::ResizeTarget 을 호출하여 애플리케이션이 모드를 
	// 전환 할 수있게하려면이 플래그를 설정하십시오.창에서 전체 화면 
	// 모드로 전환하면 응용 프로그램 창의 크기에 맞게 디스플레이 
	// 모드(또는 모니터 해상도)가 변경됩니다.

	// 전체화면을 쓰면
	// 디바이스 소실이 일어날수가 있다.
	SCINFO.Windowed = true;

	// 다이렉트 11은 다이렉트 9보다 난이도가 높고
	// 다이렉트 12 다이렉트 11보다 난이도가 높다.

	// 일단 다이렉트 11에서 사용하는 메모리들은 fmode 랑 똑같이
	// 메모리 관리도 release도 왠만하면 내가 다 직접해줘야 한다.
	// 그중에서 스왑체인은 만드는 방법이 좀 복잡하다.

	// 
	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	// "54ec77fa-1377-44e6-8c32-88fd5f44c84c"
	// 복사한다고 하더라도// 됐나????
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pD);
	if (nullptr == pD)
	{
		ASSERTMSG;
		return false;
	}

	// 디바이스를 만들고 나서 그녀석
	// IDXGIAdapter장치와 연결해서 명령을 내리는데 사용합니다. 
	pD->GetParent(__uuidof(IDXGIAdapter), (void**)&pA);
	if (nullptr == pA)
	{
		ASSERTMSG;
		return false;
	}

	// 즉 다이렉트는 메모리가 구성될때 
	pA->GetParent(__uuidof(IDXGIFactory), (void**)&pF);
	if (nullptr == pF)
	{
		ASSERTMSG;
		return false;
	}

	// m_pDevice

	if (S_OK != pF->CreateSwapChain(m_pDevice, &SCINFO, &m_pSwapChain))
	{
		ASSERTMSG;
		return false;
	}

	pF->Release();
	pA->Release();
	pD->Release();
	return true;
}

bool GameDevice::CreateDS()
{
	// 0~ 1을 기록하는 버퍼 모니터 크기만한 버퍼는 뭐로 만드느냐?
	// 이미지는 이이제 그냥 2차원 배열을 선언해놨다고 생각하는게 편하다. 
	// ID3D11Texture2D* pBackBufferTex = nullptr;

	//__uuidof : 식에 연결된 GUID를 검색합니다.
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBufferTex))
	{
		ASSERTMSG;
		return false;
	}

	// 여러분들이 텍스처를 만들어도 
	// 그래픽카드 연산에서 쓰일수 있는 녀석이 아니에요.
	// 그래픽카드한테 어떤 텍스처가 그래픽 연산을 위해서 쓰일거야.
	// 백버퍼이기 때문에 랜더링에 사용하려고 만든것은 아니다.
	// 지울때도 이 뷰권한이 필요하다.
	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBufferTex, 0, &m_pRTV))
	{
		ASSERTMSG;
		return false;
	}

	// 참고로 여러분들이 자동으로 리소인 텍스처를 로드하면
	// 이 정보는 알아서 세팅된다.
	D3D11_TEXTURE2D_DESC TD;
	GameTFunc::ZERO(TD);
	// 너비와 크기가 같은 텍스처 여러장이 겹쳐있는 것처럼 사용가능.
	// 이건 멀티텍스처를 만들수 있다.
	TD.ArraySize = 1;
	TD.Width = m_ParentWin->Size().uix();
	TD.Height = m_ParentWin->Size().uiy();

	// 3바이트 float과 1바이트 언사인드 int로 구성
	// 이때 용도는 float은
	// 깊이 저장
	// int는 스텐실 버퍼를 저장하는데
	// 이 스텐실버퍼는
	// 0~255사이의 값으로 픽셀에 기록후
	// 그 값을 이용할수 있다.
	// a라는 물체가 어떤 그림그린다음
	// 스텐실버퍼에 5를 써놓으면
	// 다음에 그리는 애들이 스텐실 버퍼가 5인 애는
	// 어떻게 어떻게 처리해줘 라는 짓을 할수가 있다.
	TD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 깊이만 체크하는 녀석이라 퀄리와 카운트가 필요가 없군요.
	TD.SampleDesc.Count = 1;
	TD.SampleDesc.Quality = 0;
	TD.MipLevels = 1;

	// D3D11_USAGE_DYNAMIC
	// 무조건 너는 그래픽카드 안에서 있어야해
	TD.Usage = D3D11_USAGE_DEFAULT;
	// 텍스처 용도
	// 깊이 버퍼로 쓰겠다.
	TD.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	m_pDevice->CreateTexture2D(&TD, 0, &m_pDepthStencilTex);
	if (nullptr == m_pDepthStencilTex)
	{
		ASSERTMSG;
		return false;
	}

	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDSV);
	if (nullptr == m_pDSV)
	{
		ASSERTMSG;
		return false;
	}

	return true;
	// 이건 아직 그리고 깊어버퍼 만든거 아니다.

}


bool GameDevice::CreateVP()
{
	//초기화 지점
	m_VP.TopLeftX = 0;
	m_VP.TopLeftY = 0;
	//z 깊이 설정
	m_VP.MinDepth = 0.0f;
	m_VP.MaxDepth = 1.0f;

	//너비 설정
	m_VP.Width = m_ParentWin->Size().x;
	m_VP.Height = m_ParentWin->Size().y;

	return true;
}


void GameDevice::RenderBegin()
{
	if (nullptr != m_pContext)
	{
		m_pContext->ClearRenderTargetView(m_pRTV, m_ClearColor.Arr);
		// 0.0f 
		// 제발 인간분들아 설명을 잘들으세요.
		// 이걸 , 1.0f = 0.0f로 초기화해놓고 이렇게 강조했는데 하면
		// 내가 무슨짓을 몰라.
		// z값 깊이값을 의미하며 이 값보다 작은 값들을 대상으로 래스터라이저가 그림을 그린다.
		// 즉 이 값이 최대값이 아니고 최소값이면 모든 그림을 그릴 수 없다.
		m_pContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GameDevice::RenderEnd()
{
	if (nullptr != m_pSwapChain)
	{
		m_pSwapChain->Present(0, 0);
	}
}


void GameDevice::DefInit()
{
	ShaderInit();
	SmpInit();
	BlendInit();
	RasterizerInit();
	MeshInit();
	DepthStencilInit();

	//랜더패스는 모든 Init이 진행된 후에 설정되어야 한다.
	RenderPathInit();
	FontInit();
	GameDebugEn::Init();

}

void GameDevice::DEFVIEWPORTSET()
{
	//랜더링 관련 부분은 Context가 관여함
	m_pContext->RSSetViewports(1, &m_VP);
}

void GameDevice::DEFRENDERTARGETSET()
{
	// 깊이버퍼는 m_pDSV;
	// 랜더 타겟은 m_pRTV 백버퍼 랜더타겟
	m_pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
}
