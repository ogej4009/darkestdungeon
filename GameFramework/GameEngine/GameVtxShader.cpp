#include "GameVtxShader.h"



//최초 생성시에 부모포인터를 저장해 두므로 this로 저장
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
	// 버전을 문자열로 알려줘야 한다.

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
	// _MAC은 배열로 들어갈수 있다.
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


//_INPUT_ELEMENT_DESC를 추가해주는것
//EndLayOut전까지 AddLayOut은 쉐이더 각각의 시맨틱마다 하나씩 선언되야하고
//마지막 시맨틱을 EndLayOut으로 넣어주면 된다.
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

	// 아래둘은 인스턴싱이라는 것을 배우기 전까지 거의 쓸일이 없다.
	InputData.InstanceDataStepRate = _IDSR;
	InputData.InputSlotClass = _InputClass;

	InputData.AlignedByteOffset = m_Offset;

	// m_Offset 이녀석의 사이즈만큼 증가할것이다. 
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