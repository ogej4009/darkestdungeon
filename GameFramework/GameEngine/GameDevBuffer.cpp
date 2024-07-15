#include "GameDevBuffer.h"
#include <GameTFunc.h>


GameDevBuffer::GameDevBuffer() : m_pBuffer(nullptr)
{
	GameTFunc::ZERO(m_BufferDesc);
}


GameDevBuffer::~GameDevBuffer()
{
	if (nullptr != m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
	}
}
