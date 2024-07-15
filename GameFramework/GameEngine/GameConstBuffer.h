#pragma once
#include "GameDevBuffer.h"
#include "GameShader.h"
#include <GameRTTI.h>
#include <vector>

class GameConstBuffer : public GameRes<GameConstBuffer>, public GameDevBuffer, public GameRTTI
{
private:
	bool m_bLink;
	GameShader::SHADER_TYPE m_SHADER;
	//슬롯번호 쉐이더의 몇번 슬롯에 넣을것인가
	unsigned int m_uiReg; // b0 b1 b2
	void (GameConstBuffer::*m_UpdateFunc)();

public:
	GameConstBuffer();
	~GameConstBuffer();

public:
	unsigned int Slot()
	{
		return m_uiReg;
	}

	template<typename T>
	void SettingData(T& _Data) 
	{
		SettingData((void*)&_Data, sizeof(T));
	}

	void SettingData(void* _Ptr, unsigned int _Size);
	bool Create(GameShader::SHADER_TYPE _SHADER, unsigned int _uiReg, unsigned int _Size);
	void SettingInit(GameShader::SHADER_TYPE _SHADER, unsigned int _uiReg);
	void Update(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업

private:
	void VSUpdate(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업
	void HSUpdate(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업
	void DSUpdate(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업
	void GSUpdate(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업
	void PSUpdate(); // 상수버퍼가 자신이 가진 데이터를 그래픽카드에 넘겨주는 작업

};

