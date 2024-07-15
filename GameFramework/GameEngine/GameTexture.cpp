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
		// �ε��Լ��� �ٸ���.
	}
	else if (m_Path.PathData().IsExt(L"TGA") || m_Path.PathData().IsExt(L"tga"))
	{
		// �ε��Լ��� �ٸ���.
	}
	else
	{
		// �׷��� ������ �÷��� �� float�̴�. 
		// �ְ� ���ĸ� ũ�� ���� ������ �����ֵ�
		// DirectX::LoadFromHDRFile
		if (S_OK != DirectX::LoadFromWICFile(m_Path, DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			ASSERTMSG;
			return false;
		}
	}

	// �ε�ʹ� ������
	// ���̴��� �־��ַ���
	// �� �ؽ�ó�� ��ȭ���� �������
	// �̰��� �ʿ��ϴ�.
	// ID3D11RenderTargetView*		  m_pRTV;
	// ���⼭�� �̰� ���� �ȸ�����.
	// �� �̷��� ������� �������ϱ�. 

	// ID3D11DepthStencilView ����̽� ������.
	// �ִ�. 
	// Z�� 
	// ID3D11DepthStencilView*		  m_pDSV;


	// ���̴� �����ؼ� ������ ������ �Ѵ�.
	// ID3D11ShaderResourceView*		  m_pSRV;


	//���� ���ҽ� ��� ����� ��� ��
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
