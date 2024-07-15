#include "GameShader.h"
#include "GameConstBuffer.h"
#include <GameFile.h>

GameShader::GameShader(SHADER_TYPE _Type) : m_Type(_Type)
{
}


GameShader::~GameShader()
{
	if (nullptr != m_ErrBlob)
	{
		m_ErrBlob->Release();
	}

	if (nullptr != m_Blob)
	{
		m_Blob->Release();
	}
}

void GameShader::SShaderDataParsing(GameString& _Path)
{
	GameFile NewFile = GameFile(_Path, L"rt");

	//��� �߰��� ���� ���Ѵ�.
	//���ؽ� ���̴��� ����ִ°�?
	//�ȼ� ���̴��� ����ִ°�? �� üũ�ϴµ�

	GameString Str = NewFile.GetFileToText();

	std::map<GameString, StructData> AllStructData;
	GameString TempStr = Str;

	//struct�� �����ϴ� ������ŭ �߰��ǰ�
	//ã�� �ε����� ���빰�� �����س���
	std::vector<size_t> StructCount = Str.FindToCount(L"struct");

	for (size_t i = 0; i < StructCount.size(); i++)
	{
		//wstring�� find�� �����µ� ���� �����ϴ�.
		size_t End = Str.m_Str.find(L"}", StructCount[i]);

		//Struct �м�
		{
			GameString StructText = Str.GetCutText(StructCount[i], End);
			size_t NameEnd = StructText.Find(L"{");
			GameString Name = StructText.GetCutText(0, NameEnd);
			Name.ChangeToAll(L"", L"struct",L"{");

			StructData SD = StructData();
			SD.Name = Name;

			AllStructData.insert(std::map<GameString, StructData>::value_type(Name, SD));
			//ã�Ƽ� �����س��� ������ �����.
			TempStr.Change(StructText, L"");
		}
		
	}

	Str = TempStr;

	//������ Struct�� ������ �Լ� ã��
	//�̸��� ������ ������ ���� first�� ã�� ����
	for (auto& _Struct : AllStructData)
	{
		std::vector<size_t> FindCount = Str.FindToCount(_Struct.first);
	
		for (size_t i = 0; i < FindCount.size(); i++)
		{
			if (L'(' == Str[FindCount[i] - 1]
				&& L')' != Str[FindCount[i] + _Struct.first.Size()])
			{
				int a = 0;
			}
		}
	}
}


//
//void GameShader::CreateCB(unsigned int _uiReg, unsigned int _Size)
//{
//	GameConstBuffer* NewCB = new GameConstBuffer();
//	NewCB->Create(m_Type, _uiReg, _Size);
//
//	m_CB.resize(_uiReg + 1);
//
//	m_CB[_uiReg] = NewCB;
//}
//
//
//void GameShader::SettingData(unsigned int _uiReg, void* _Data, unsigned int _Size)
//{
//	if (m_CB.size() <= _uiReg)
//	{
//		ASSERTMSG;
//	}
//
//	m_CB[_uiReg]->SettingData(_Data, _Size);
//}
//
//void GameShader::SettingCB()
//{
//	// ranged for
//
//	for (SPtr<GameConstBuffer>& Data : m_CB)
//	{
//		if (nullptr == Data)
//		{
//			continue;
//		}
//		Data->Update();
//	}
//
//
//}

// �̽��̴��� ����Ϸ��� �̷� ������۵��
void GameShader::CB(const GameString& _Name, unsigned int _uiReg, unsigned int _Size, const type_info* _Type)
{
	GameConstBuffer* NewCB = new GameConstBuffer();
	NewCB->Create(m_Type, _uiReg, _Size);
	NewCB->TypeSetting(_Type);

	m_CB.insert(std::map<GameString, SPtr<GameConstBuffer>>::value_type(_Name, NewCB));
}

// �̷� �ؽ�ó��
void GameShader::TEX(const GameString& _Name, unsigned int _uiReg) {
	m_TEX.insert(std::map<GameString, unsigned int>::value_type(_Name, _uiReg));
}
// �̷� ���÷����� ��������� �Ѵ�.
void GameShader::SMP(const GameString& _Name, unsigned int _uiReg) {
	m_SMP.insert(std::map<GameString, unsigned int>::value_type(_Name, _uiReg));
}