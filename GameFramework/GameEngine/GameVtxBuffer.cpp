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

	//���� ������ �ϸ� �����߿� ���� ���氡��
	//���� Ŀ���͸���¡ ����̶�� ���� �ȴ�.
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_BufferDesc.Usage)
	{
		//�� ������� ����Ϸ��� CPU������ �÷��׸� write�� ������ ��� �Ѵ�.
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = D3D11_SUBRESOURCE_DATA();
	//memset Zero �� �Ű����� �Ҵ�����
	GameTFunc::ZERO(DS);
	//_Data�� ���� �� ������ ����, �ؿ� RS�� �Ű������� ���� �ڷᰡ ������ �a�ϰ� ���� �־������
	DS.pSysMem = _Data;

	HRESULT RS = GameDevice::MainDevice()->CreateBuffer(&m_BufferDesc, &DS, &m_pBuffer);
	if (S_OK != RS)
	{
		ASSERTMSG;
		return false;
	}


	return true;
}