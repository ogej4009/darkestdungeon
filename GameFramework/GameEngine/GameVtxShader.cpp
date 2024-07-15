#include "GameVtxShader.h"



//���� �����ÿ� �θ������͸� ������ �ιǷ� this�� ����
GameVtxShader::GameVtxShader() : LayOut(this), GameShader(SHADER_TYPE::ST_VS)
{

}


GameVtxShader::~GameVtxShader()
{
	if (nullptr != m_pVTXPTR)
	{
		m_pVTXPTR->Release();
	}
}


// 5.0 
bool GameVtxShader::Load(unsigned int _VH, unsigned int _VL, const GameString& _FuncName, D3D_SHADER_MACRO _MAC, ID3DInclude* _Inc)
{
	VerSetting(_VH, _VL);
	// ������ ���ڿ��� �˷���� �Ѵ�.

	unsigned int _Flag = 0;

#ifdef _DEBUG
	_Flag = D3D10_SHADER_DEBUG;
#endif
	// HGAMESTRING Target;
	char Arr[256] = { 0 };
	sprintf_s(Arr, "vs_%d_%d", _VH, _VL);

	// Target.Create(L"vs_%d_%d", _VH, _VL);
	std::string Name = _FuncName;

	SShaderDataParsing(m_Path);
	// _MAC�� �迭�� ���� �ִ�.
	HRESULT RS = D3DCompileFromFile(m_Path, &_MAC, _Inc, Name.c_str(), Arr, _Flag, 0, &m_Blob, &m_ErrBlob);

	if (S_OK != RS)
	{
		std::string Error = (char*)m_ErrBlob->GetBufferPointer();

		ASSERTMSG;
		return false;
	}

	RS = GameDevice::MainDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_pVTXPTR);

	if (nullptr == m_pVTXPTR)
	{
		ASSERTMSG;
		return false;
	}

	return true;
}


//_INPUT_ELEMENT_DESC�� �߰����ִ°�
//EndLayOut������ AddLayOut�� ���̴� ������ �ø�ƽ���� �ϳ��� ����Ǿ��ϰ�
//������ �ø�ƽ�� EndLayOut���� �־��ָ� �ȴ�.
void GameVtxShader::SINPUTLAYOUT::AddLayOut(
	const char* _SmtName,
	unsigned int _Index,
	DXGI_FORMAT _Format,
	unsigned int _inputSlot,
	unsigned int _IDSR /*= 0*/,
	D3D11_INPUT_CLASSIFICATION _InputClass /*= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/
)
{
	if (nullptr != m_LayOut)
	{
		ASSERTMSG;
	}

	D3D11_INPUT_ELEMENT_DESC InputData;

	InputData.SemanticName = _SmtName; // POSITIOPN
	InputData.SemanticIndex = _Index; // 0 
	InputData.Format = _Format; // R32G32
	InputData.InputSlot = _inputSlot; // 0 ~ 15 POSITION0, POSITION1

	// �Ʒ����� �ν��Ͻ��̶�� ���� ���� ������ ���� ������ ����.
	InputData.InstanceDataStepRate = _IDSR;
	InputData.InputSlotClass = _InputClass;

	InputData.AlignedByteOffset = m_Offset;

	// m_Offset �̳༮�� �����ŭ �����Ұ��̴�. 
	m_Offset += GameMath::FormatByteSize(_Format);
	m_ArrInputData.push_back(InputData);
}


void GameVtxShader::SINPUTLAYOUT::EndLayOut(
	const char* _SmtName,
	unsigned int _Index,
	DXGI_FORMAT _Format,
	unsigned int _inputSlot,
	unsigned int _IDSR /*= 0*/,
	D3D11_INPUT_CLASSIFICATION _InputClass /*= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/
)
{
	AddLayOut(_SmtName, _Index, _Format, _inputSlot, _IDSR, _InputClass);

	HRESULT RS = GameDevice::MainDevice()->CreateInputLayout(
		&m_ArrInputData[0],
		(UINT)m_ArrInputData.size(),
		m_pParent->m_Blob->GetBufferPointer(),
		m_pParent->m_Blob->GetBufferSize(),
		&m_LayOut);

	if (S_OK != RS)
	{
		ASSERTMSG;
		return;
	}

	return;
}


void GameVtxShader::Setting()
{
	if (nullptr != LayOut.m_LayOut)
	{
		GameDevice::MainContext()->IASetInputLayout(LayOut.m_LayOut);
	}

	if (nullptr != m_pVTXPTR)
	{
		GameDevice::MainContext()->VSSetShader(m_pVTXPTR, nullptr, 0);
	}
}