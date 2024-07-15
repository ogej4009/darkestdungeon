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
		//상수버퍼 타입
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
		ST_VS, //버텍스 쉐이더
		ST_HS, //훌 쉐이더
		// ST_TS,
		ST_DS, //도메인 쉐이더
		ST_GS, //지오메트리 쉐이더
		ST_PS, //픽셀 쉐이더
		ST_MAX,
	};

protected:
	// 쉐이더 컴파일을 하고 나온 정보를 보관하는 보관체
	// string
	ID3DBlob* m_Blob;
	ID3DBlob* m_ErrBlob;
	unsigned int m_VH; // 쉐이더 버전용 5
	unsigned int m_VL;
	SHADER_TYPE m_Type;

private:
	std::map<GameString, SPtr<GameConstBuffer>> m_CB;

	//샘플러,텍스쳐의 이름과 슬롯 번호 저장
	std::map<GameString, unsigned int> m_SMP;
	std::map<GameString, unsigned int> m_TEX;

protected:
	GameShader(SHADER_TYPE _Type);
	virtual ~GameShader();

	//해당 쉐이더 버전 넣어주는 용도
public:
	SHADER_TYPE ShType()
	{
		return m_Type;
	}

	void SShaderDataParsing(GameString& _Path);

	//완전 가상 함수 추상클래스 만듦
	virtual void Setting() = 0;

	void VerSetting(unsigned int _VH, unsigned int _VL) {
		m_VH = _VH;
		m_VL = _VL;
	}

	//상수버퍼의 크기를 템플릿으로 받아서 상수버퍼를 만듦
	template<typename T>
	void CB(const GameString& _Name, unsigned int _uiReg)
	{
		CB(_Name, _uiReg, sizeof(T), &typeid(T));
	}

	void CB(const GameString& _Name, unsigned int _uiReg, unsigned int _Size, const type_info* _Type);
	void TEX(const GameString& _Name, unsigned int _uiReg);
	void SMP(const GameString& _Name, unsigned int _uiReg);

	/* 다수의 상수버퍼를 사용하기 위해서 맵으로 관리되는 CB 형태로 변경함
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

