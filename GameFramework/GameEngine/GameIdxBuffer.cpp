#include "GameIdxBuffer.h"
#include <GameTFunc.h>


GameIdxBuffer::GameIdxBuffer()
{
}


GameIdxBuffer::~GameIdxBuffer()
{
}


// DXGI_FORMAT _Fmt // 언사인드 INT인지 아니면 2바이트 인트인지 포맷
// UINT _iDXCount,  // 인덱스 개수
// UINT _iDXSize,  // 언사인드 INT인지 아니면 2바이트 인트인지
// D3D11_USAGE _VTXUSAGE, 
// void* _Data
bool GameIdxBuffer::Create(DXGI_FORMAT _Fmt, UINT _iDXCount, UINT _iDXSize, D3D11_USAGE _VTXUSAGE, void* _Data)
{
	// 어차피 이렇게 고정시킬거고 고정시켜도 되는데 왜 이렇게 하느냐?
	// 선생님이나 그런 사람들은 인간믿지 않는다.
	// 그러므로 애초에 어떤식으로 데이터가 들어오더라도 알아서 판단해서
	// 짜는 코드를 만들고 싶다.
	// 그런 예외상황이 오면
	// 내가 바로 알수 있는 코드를 쳐놓는것.

	m_IDXFmt = _Fmt;
	m_IDXSize = _iDXSize;
	m_IDXCount = _iDXCount;

	m_BufferDesc.ByteWidth = m_IDXCount * m_IDXSize;
	m_BufferDesc.Usage = _VTXUSAGE;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_BufferDesc.Usage)
	{
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = D3D11_SUBRESOURCE_DATA();
	GameTFunc::ZERO(DS);
	DS.pSysMem = _Data;

	HRESULT RS = GameDevice::MainDevice()->CreateBuffer(&m_BufferDesc, &DS, &m_pBuffer);
	if (S_OK != RS)
	{
		ASSERTMSG;
		return false;
	}

	return true;
}