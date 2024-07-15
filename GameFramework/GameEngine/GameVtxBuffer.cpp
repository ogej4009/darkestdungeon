#include "GameVtxBuffer.h"
#include <GameTFunc.h>


GameVtxBuffer::GameVtxBuffer()
{
}

GameVtxBuffer::GameVtxBuffer(const GameString& _Name, UINT _VTXCOUNT, UINT _VTXSIZE, D3D11_USAGE _VTXUSAGE, void* _DATA)
{
	AddRes(_Name, this);
	Create(_VTXCOUNT, _VTXSIZE, _VTXUSAGE, _DATA);
}


GameVtxBuffer::~GameVtxBuffer()
{
}


bool GameVtxBuffer::Create(UINT _VTXCOUNT, UINT _VTXSIZE, D3D11_USAGE _VTXUSAGE, void* _Data)
{
	m_VTXSize = _VTXSIZE;
	m_VTXCount = _VTXCOUNT;
	m_BufferDesc.ByteWidth = m_VTXSize * m_VTXCount;
	m_BufferDesc.Usage = _VTXUSAGE;

	//동적 세팅을 하면 실행중에 값을 변경가능
	//게임 커스터마이징 기능이라고 보면 된다.
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_BufferDesc.Usage)
	{
		//위 방식으로 사용하려면 CPU엑세스 플래그를 write로 설정해 줘야 한다.
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = D3D11_SUBRESOURCE_DATA();
	//memset Zero 후 매개변수 할당해줌
	GameTFunc::ZERO(DS);
	//_Data는 실제 점 정보를 말함, 밑에 RS의 매개변수로 들어갈때 자료가 없으면 퇫하고 뱉어내서 넣어줘야함
	DS.pSysMem = _Data;

	HRESULT RS = GameDevice::MainDevice()->CreateBuffer(&m_BufferDesc, &DS, &m_pBuffer);
	if (S_OK != RS)
	{
		ASSERTMSG;
		return false;
	}


	return true;
}