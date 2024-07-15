#pragma once
#include "GameDevice.h"
#include <vector>

class GameRenderData;
class GameConstBuffer;
class GameShader
{
public:
	friend GameRenderData;

public:
	/*class CBDATA
	{
	public:
		unsigned int Size;
		unsigned int Reg;
	};*/

	class MemberData
	{
	public:
		//������� Ÿ��
		GameString DataTypeName;
		size_t TypeSize;
	};

	class StructData
	{
	public:
		GameString Name;
		size_t DataSize;
		std::vector<MemberData> MemberData;
	};

	enum class SHADER_TYPE
	{
		ST_VS, //���ؽ� ���̴�
		ST_HS, //�� ���̴�
		// ST_TS,
		ST_DS, //������ ���̴�
		ST_GS, //������Ʈ�� ���̴�
		ST_PS, //�ȼ� ���̴�
		ST_MAX,
	};

protected:
	// ���̴� �������� �ϰ� ���� ������ �����ϴ� ����ü
	// string
	ID3DBlob* m_Blob;
	ID3DBlob* m_ErrBlob;
	unsigned int m_VH; // ���̴� ������ 5
	unsigned int m_VL;
	SHADER_TYPE m_Type;

private:
	std::map<GameString, SPtr<GameConstBuffer>> m_CB;

	//���÷�,�ؽ����� �̸��� ���� ��ȣ ����
	std::map<GameString, unsigned int> m_SMP;
	std::map<GameString, unsigned int> m_TEX;

protected:
	GameShader(SHADER_TYPE _Type);
	virtual ~GameShader();

	//�ش� ���̴� ���� �־��ִ� �뵵
public:
	SHADER_TYPE ShType()
	{
		return m_Type;
	}

	void SShaderDataParsing(GameString& _Path);

	//���� ���� �Լ� �߻�Ŭ���� ����
	virtual void Setting() = 0;

	void VerSetting(unsigned int _VH, unsigned int _VL) {
		m_VH = _VH;
		m_VL = _VL;
	}

	//��������� ũ�⸦ ���ø����� �޾Ƽ� ������۸� ����
	template<typename T>
	void CB(const GameString& _Name, unsigned int _uiReg)
	{
		CB(_Name, _uiReg, sizeof(T), &typeid(T));
	}

	void CB(const GameString& _Name, unsigned int _uiReg, unsigned int _Size, const type_info* _Type);
	void TEX(const GameString& _Name, unsigned int _uiReg);
	void SMP(const GameString& _Name, unsigned int _uiReg);

	/* �ټ��� ������۸� ����ϱ� ���ؼ� ������ �����Ǵ� CB ���·� ������
	template<typename T>
	void CreateCB(unsigned int _uiReg)
	{
		CreateCB(_uiReg, sizeof(T));
	}

	void CreateCB(unsigned int _uiReg, unsigned int _Size);
*/
	/*template<typename T>
	void SettingData(unsigned int _uiReg, T& _Data)
	{
		SettingData(_uiReg, (void*)&_Data, sizeof(T));
	}

	void SettingData(unsigned int _uiReg, void* _Data, unsigned int _Size);

	void SettingCB();*/


};

