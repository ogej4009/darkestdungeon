#include "GameMesh.h"
#include "GameDevice.h"


GameMesh::GameMesh()
{
}


GameMesh::~GameMesh()
{
}


bool GameMesh::Create(const std::vector<GameString>& _VBList, const std::vector<GameString>& _IBList)
{
	for (size_t i = 0; i < _VBList.size(); i++)
	{
		AddVB(_VBList[i]);
	}

	for (size_t i = 0; i < _IBList.size(); i++)
	{
		AddIB(_IBList[i]);
	}
	return true;
}


bool GameMesh::Create(const wchar_t* _VB, const wchar_t* _IB)
{
	AddVB(_VB);
	AddIB(_IB);
	return true;
}


void GameMesh::AddIB(const wchar_t* _Name)
{
	SPtr<GameIdxBuffer> Ptr = GameIdxBuffer::Find(_Name);

	if (nullptr == Ptr)
	{
		ASSERTMSG;
	}

	m_ArrIBBuffer.push_back(Ptr->Buffer());
	m_ArrIBOffset.push_back(0);
	m_ArrIB.push_back(Ptr);
}


void GameMesh::AddVB(const wchar_t* _Name)
{
	SPtr<GameVtxBuffer> Ptr = GameVtxBuffer::Find(_Name);

	if (nullptr == Ptr)
	{
		ASSERTMSG;
	}

	m_ArrVBBuffer.push_back(Ptr->Buffer());
	m_ArrVBSize.push_back(Ptr->VTXSize());
	m_ArrVBOffset.push_back(0);
	m_ArrVB.push_back(Ptr);
}


void GameMesh::SettingVB(UINT Index)
{
	// IA Input Assembler ��ǲ �����
	// OM Output Merger �ƿ�ǲ ����.
	// HGAMEDEVICE::MainContext()->OMGetBlendState
	// 5���� ���������.
	//   1 2
	// 0 1 2 3 4
		//_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot, //  ���۽��� 
		//_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers, ���(�ڱ��ڽ� ����)
		//_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,//���� ������ �־�a
		//_In_reads_opt_(NumBuffers)  const UINT *pStrides, // 46����Ʈ ¥�� ���ؽ���. ���ϳ��� �����Ʈ��
		//_In_reads_opt_(NumBuffers)  const UINT *pOffsets	// ���ؽ� ������ ���۹���
	// 
	GameDevice::MainContext()->IASetVertexBuffers(0, 1, &m_ArrVBBuffer[Index], &m_ArrVBSize[Index], &m_ArrVBOffset[Index]);
}

void GameMesh::SettingIB(UINT Index)
{
	GameDevice::MainContext()->IASetIndexBuffer(m_ArrIBBuffer[Index], m_ArrIB[Index]->IDXFmt(), m_ArrIBOffset[0]);
}

void GameMesh::RenderIndexed(UINT _Index)
{
	//�ε��� ���ۿ��� ���� �����ͼ� �׷���
	GameDevice::MainContext()->DrawIndexed(m_ArrIB[_Index]->IDXCount(), 0, 0);
}
