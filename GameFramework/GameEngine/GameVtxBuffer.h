#pragma once
#include <GameRes.h>
#include "GameDevice.h"
#include "GameDevBuffer.h"
#include <vector>

class GameVtxBuffer : public GameRes<GameVtxBuffer>, public GameDevBuffer
{
private:
	UINT m_VTXSize;
	UINT m_VTXCount;


public:
	GameVtxBuffer();
	GameVtxBuffer(const GameString& _Name, UINT _VTXCOUNT, UINT _VTXSIZE, D3D11_USAGE _VTXUSAGE, void* _DATA);
	~GameVtxBuffer();

public:
	template<typename T>
	static bool Create(const GameString& _Name, std::vector<T>& _Vec, D3D11_USAGE _VTXUSAGE)
	{
		if (0 >= _Vec.size())
		{
			ASSERTMSG;
			return false;
		}

		GameVtxBuffer* NewRes = CreateRes(_Name);
		return NewRes->Create((UINT)_Vec.size(), sizeof(_Vec[0]), _VTXUSAGE, &_Vec[0]);
	}


	static bool Create(const GameString& _Name, UINT _VTXCOUNT, UINT _VTXSIZE, D3D11_USAGE _VTXUSAGE, void* _DATA)
	{
		GameVtxBuffer* NewRes = CreateRes(_Name);
		return NewRes->Create(_VTXCOUNT, _VTXSIZE, _VTXUSAGE, _DATA);
	}

	bool Create(UINT _VTXCOUNT, UINT _VTXSIZE, D3D11_USAGE _VTXUSAGE, void* _Data);


	UINT VTXSize()
	{
		return m_VTXSize;
	}

	UINT VTXCount()
	{
		return m_VTXCount;
	}
};

