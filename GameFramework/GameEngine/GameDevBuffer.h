#pragma once
#include <GameRes.h>
#include "GameDevice.h"

class GameDevBuffer
{
protected:
	D3D11_MAPPED_SUBRESOURCE m_SubMap;
	D3D11_BUFFER_DESC m_BufferDesc;
	ID3D11Buffer* m_pBuffer;

	//�� �����͸� �پ��� Create�Լ��� �־���� �Ѵ�.
	//���������� �䱸��
	//�׷��� ī���� �޸𸮿� �����ϱ� ���� �ڵ��̶�� ������ ����

public:
	GameDevBuffer();
	virtual ~GameDevBuffer();

public:
	ID3D11Buffer* Buffer()
	{
		return m_pBuffer;
	}

	D3D11_BUFFER_DESC BufferDesc()
	{
		return m_BufferDesc;
	}

};

