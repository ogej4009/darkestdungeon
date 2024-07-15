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
	//���Թ�ȣ ���̴��� ��� ���Կ� �������ΰ�
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
	void Update(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�

private:
	void VSUpdate(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�
	void HSUpdate(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�
	void DSUpdate(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�
	void GSUpdate(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�
	void PSUpdate(); // ������۰� �ڽ��� ���� �����͸� �׷���ī�忡 �Ѱ��ִ� �۾�

};

