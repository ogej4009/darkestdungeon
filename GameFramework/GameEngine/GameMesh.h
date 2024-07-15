#pragma once
#include "GameVtxBuffer.h"
#include "GameIdxBuffer.h"

class GameMesh : public GameRes<GameMesh>
{
private:
	//���ؽ� ���ۿ� �ε��� ���� ���
	std::vector<SPtr<GameVtxBuffer>> m_ArrVB;
	std::vector<SPtr<GameIdxBuffer>> m_ArrIB;

	//����context�� �Է°��� ���ؼ� ���� �����صδ� ��ϵ�
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

	//���鶧 ���ؽ� ���ۿ� �ε��� ������ ����� ������ �޾Ƽ� ���� �� ��Ͻ�Ű�� ������ �Լ�
	static bool Create(const GameString& _Name, const std::vector<GameString>& _VBList, const std::vector<GameString>& _IBList)
	{
		GameMesh* NewRes = CreateRes(_Name);
		return NewRes->Create(_VBList, _IBList);
	}

	//���鶧 ���ؽ� ���ۿ� �ε��� ������ ����� �ϳ��� �޾Ƽ� ��Ͻ�Ű�� ������ �Լ�
	static bool Create(const GameString& _Name, const wchar_t* _VB, const wchar_t* _IB)
	{
		GameMesh* NewRes = CreateRes(_Name);
		return NewRes->Create(_VB, _IB);
	}

};

