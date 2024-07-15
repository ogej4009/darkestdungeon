#pragma once
#include "GameVtxBuffer.h"
#include "GameIdxBuffer.h"

class GameMesh : public GameRes<GameMesh>
{
private:
	//버텍스 버퍼와 인덱스 버퍼 목록
	std::vector<SPtr<GameVtxBuffer>> m_ArrVB;
	std::vector<SPtr<GameIdxBuffer>> m_ArrIB;

	//메인context의 입력값을 위해서 따로 저장해두는 목록들
	std::vector<ID3D11Buffer*> m_ArrVBBuffer;
	std::vector<UINT> m_ArrVBSize;
	std::vector<UINT> m_ArrVBOffset;

	std::vector<ID3D11Buffer*> m_ArrIBBuffer;
	std::vector<UINT> m_ArrIBOffset;


public:
	GameMesh();
	~GameMesh();

public:
	bool Create(const std::vector<GameString>& _VBList, const std::vector<GameString>& _IBList);
	bool Create(const wchar_t* _VB, const wchar_t* _IB);

	void AddIB(const wchar_t* _Name);
	void AddVB(const wchar_t* _Name);

	void SettingVB(UINT Index = 0);
	void SettingIB(UINT Index = 0);

	void RenderIndexed(UINT _Index);

	//만들때 버텍스 버퍼와 인덱스 버퍼의 목록을 여러개 받아서 각자 다 등록시키는 형태의 함수
	static bool Create(const GameString& _Name, const std::vector<GameString>& _VBList, const std::vector<GameString>& _IBList)
	{
		GameMesh* NewRes = CreateRes(_Name);
		return NewRes->Create(_VBList, _IBList);
	}

	//만들때 버텍스 버퍼와 인덱스 버퍼의 목록을 하나만 받아서 등록시키는 형태의 함수
	static bool Create(const GameString& _Name, const wchar_t* _VB, const wchar_t* _IB)
	{
		GameMesh* NewRes = CreateRes(_Name);
		return NewRes->Create(_VB, _IB);
	}

};

