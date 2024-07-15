#include "GameTexture.h"



GameTexture::GameTexture() : m_pSRV(nullptr)
{
}


GameTexture::~GameTexture()
{
	if (nullptr != m_pRTV)
	{
		m_pRTV->Release();
	}
	if (nullptr != m_pDSV)
	{
		m_pDSV->Release();
	}
	if (nullptr != m_pSRV)
	{
		m_pSRV->Release();
	}
	if (nullptr != m_pTex)
	{
		m_pTex->Release();
	}
}


bool GameTexture::Load() {

	m_Path;

	if (m_Path.PathData().IsExt(L"DDS") || m_Path.PathData().IsExt(L"dds"))
	{
		// 로드함수가 다르다.
	}
	else if (m_Path.PathData().IsExt(L"TGA") || m_Path.PathData().IsExt(L"tga"))
	{
		// 로드함수가 다르다.
	}
	else
	{
		// 그렇기 때문에 컬러가 다 float이다. 
		// 애가 뭐냐면 크고 색상 범위도 넓은애들
		// DirectX::LoadFromHDRFile
		if (S_OK != DirectX::LoadFromWICFile(m_Path, DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			ASSERTMSG;
			return false;
		}
	}

	// 로드와는 별개로
	// 쉐이더에 넣어주려면
	// 이 텍스처를 도화지로 만들려면
	// 이것이 필요하다.
	// ID3D11RenderTargetView*		  m_pRTV;
	// 여기서는 이걸 보통 안만들지.
	// 왜 이렇게 사용하지 않을꺼니까. 

	// ID3D11DepthStencilView 디바이스 가봐라.
	// 있다. 
	// Z값 
	// ID3D11DepthStencilView*		  m_pDSV;


	// 쉐이더 세팅해서 색깔을 쓰려고 한다.
	// ID3D11ShaderResourceView*		  m_pSRV;


	//랜더 리소스 뷰는 만들어 줘야 함
	if (S_OK!= DirectX::CreateShaderResourceView(
		GameDevice::MainDevice(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_pSRV))
	{
		ASSERTMSG;
		return false;
	}

	m_W = m_Image.GetMetadata().width;
	m_H = m_Image.GetMetadata().height;

	return true;
}



void GameTexture::SettingTex(GameShader::SHADER_TYPE _Type, int _Index)
{
	switch (_Type)
	{
	case GameShader::SHADER_TYPE::ST_VS:
		VSUpdate(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_HS:
		HSUpdate(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_DS:
		DSUpdate(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_GS:
		GSUpdate(_Index);
		break;
	case GameShader::SHADER_TYPE::ST_PS:
		PSUpdate(_Index);
		break;
	default:
		break;
	}
}


void GameTexture::VSUpdate(int _Index) {
	GameDevice::MainContext()->VSSetShaderResources(_Index, 1, &m_pSRV);
}
void GameTexture::HSUpdate(int _Index) {
	GameDevice::MainContext()->HSSetShaderResources(_Index, 1, &m_pSRV);
}
void GameTexture::DSUpdate(int _Index) {
	GameDevice::MainContext()->DSSetShaderResources(_Index, 1, &m_pSRV);
}
void GameTexture::GSUpdate(int _Index) {
	GameDevice::MainContext()->GSSetShaderResources(_Index, 1, &m_pSRV);
}
void GameTexture::PSUpdate(int _Index) {
	GameDevice::MainContext()->PSSetShaderResources(_Index, 1, &m_pSRV);
}
