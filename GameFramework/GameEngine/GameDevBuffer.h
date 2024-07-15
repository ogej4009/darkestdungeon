#pragma once
#include <GameRes.h>
#include "GameDevice.h"

class GameDevBuffer
{
protected:
	D3D11_MAPPED_SUBRESOURCE m_SubMap;
	D3D11_BUFFER_DESC m_BufferDesc;
	ID3D11Buffer* m_pBuffer;

	//위 포인터를 다양한 Create함수에 넣어줘야 한다.
	//버퍼정보를 요구함
	//그래픽 카드의 메모리에 접근하기 위한 핸들이라고 볼수도 있음

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

