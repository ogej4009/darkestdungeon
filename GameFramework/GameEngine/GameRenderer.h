#pragma once
#include "GameTransform.h"
#include <vector>
#include "GameVtxShader.h"
#include "GamePixShader.h"
#include "GameMesh.h"
#include "GameTexture.h"
#include "GameSampler.h"
#include "GameBlend.h"
#include "GameConstBuffer.h"
#include "GameRenderMaker.h"


//������ ������ ������ ���� ������ ����

class GameDebugEn;
class GameRenderData : public GameObjRef
{
private:
	class RENDERRES
	{
	public:
		unsigned int Slot;
	};

	//RenderPath�� ������ ���� ������
	//Ŭ����ȭ ������, CB,TEX,SMP
	//�̸��� ���� ���� �߰��ҵ� ������ res�� Slot��ȣ�� �Է��س���
	class CBDATA : public RENDERRES
	{
	public:
		SPtr<GameConstBuffer> CB;
		unsigned int Size;
		//���� ��ũ�� ������ �Ұǰ�?
		bool m_IsLink;
		//�ڱ⸸�� � �÷�, Ʈ���� ������ ���
		void* Data;

	public:
		CBDATA() : Data(nullptr) {}
		~CBDATA()
		{
			Reset();
		}
	public:

		void Reset()
		{
			if (false == m_IsLink
				&& nullptr != Data)
			{
				delete[] Data;
			}
		}
	};

	class TEXDATA : public RENDERRES
	{
	public:
		SPtr<GameTexture> Tex;
	public:
		TEXDATA() : Tex(nullptr) {}
	};

	class SMPDATA : public RENDERRES
	{
	public:
		SPtr<GameSampler> Smp;
	public:
		SMPDATA() : Smp(nullptr) {}
	};

private:
	std::map<GameShader::SHADER_TYPE, std::map<GameString, CBDATA>>::iterator CBIter;
	std::map<GameString, CBDATA>::iterator CBFindIter;
	std::map < GameShader::SHADER_TYPE, std::map<GameString, SMPDATA>>::iterator SmpIter;
	std::map<GameString, SMPDATA>::iterator SmpFindIter;
	std::map<GameShader::SHADER_TYPE, std::map<GameString, TEXDATA>>::iterator TexIter;
	std::map<GameString, TEXDATA>::iterator TexFindIter;


public:
	friend GameRenderer;
	friend GameDebugEn;

	//�������� �θ� �ǰ� ���� �Ž��� �����н��� ������ ���� �� �ִ�.
	//�ڽ��� Update()���� Render()�� �����ؼ� ������ ������ �ɼ��� �������ش�.
private:
	//�θ� ������ �����ͷ� ��� �־��µ� ����� �������Ҷ� ������ �������
	//�����ϱ� ���� ����. ��� TransData�� ��� �ֵ��� �ٲ�
	//GameRenderer* m_pParent;
	CTransData* m_pData;
	SPtr<GameMesh> m_Mesh;
	SPtr<GameRenderMaker> m_RenderPath;
	//�ﰢ������ �׷��� �̷� ��带 ������ ���� ����
	D3D11_PRIMITIVE_TOPOLOGY m_DrawMode;

private:
	//�ؽ�ó �ڿ��� ������ ���� �ڷᱸ��.
	//���̴� Ÿ�԰� �̸��� ��� ����
	std::map<GameShader::SHADER_TYPE, std::map<GameString, TEXDATA>> TexRes;
	std::map<GameShader::SHADER_TYPE, std::map<GameString, CBDATA>> CbRes;
	std::map<GameShader::SHADER_TYPE, std::map<GameString, SMPDATA>> SmpRes;

public:
	GameRenderData(const GameString& _Mesh, const GameString& _Path);
	~GameRenderData();

public:

	//RenderData���� CB����, �̸�, ������ ���̴�Ÿ���� �޴´�.
	//�� ���̴� Ÿ���� �⺻ ���ڰ��� ������ �־ �Է½� Ư�� ���̴� Ÿ������
	// ���Է½� ��� ���̴��� ���鼭 ã�´�.

	//CB�� ��� Link���� ��Link��尡 �����ϴµ�
	//Link���� _Data�� ������ ���·� �����ϰ� �ƴѰ� ��ü ���ο� �����Ҵ��� ����Ű�� �����Ͱ� ����ٰ� ���� �ȴ�.
	template<typename T>
	void CB(const GameString& _Name, const T& _Data, bool _bLink = false, GameShader::SHADER_TYPE _Type = GameShader::SHADER_TYPE::ST_MAX)
	{
		CbDataCreate(_Name, (void*)&_Data, _bLink, _Type);
	}
	void SMP(const GameString& _Name, const GameString& _SmpName, GameShader::SHADER_TYPE _Type = GameShader::SHADER_TYPE::ST_MAX);
	void TEX(const GameString& _Name, const GameString& _ResName, GameShader::SHADER_TYPE _Type = GameShader::SHADER_TYPE::ST_MAX);
	void TEX(const GameString& _Name, SPtr<GameTexture> _Tex,GameShader::SHADER_TYPE _Type = GameShader::SHADER_TYPE::ST_MAX);
	void Render();
	SPtr<GameRenderMaker> RenderPath();

private:
	void RenderPathInit();
	void CbDataCreate(const GameString& _Name, void* _Data, bool _Blink = false, GameShader::SHADER_TYPE _Type = GameShader::SHADER_TYPE::ST_MAX);
};


class GameCam;
class GameMesh;
class GameRenderMgr;
class GameRenderMaker;
class GameRenderer : public GameTransform
{
private:
	friend GameRenderMgr;
	friend GameRenderData;
	int m_Order;

public:
	std::vector<SPtr<GameRenderData>> m_RenderList;

public:
	GameRenderer();
	~GameRenderer();

public:
	virtual void Init(unsigned int _Order = 0);
	virtual void SceneComInit(unsigned int _Order = 0);
	virtual void Render(SPtr<GameCam> _Cam);
	virtual void RenderUpdate(SPtr <GameCam> _Cam);

public:
	GameRenderData* CreateRenderData(const GameString& _MeshName, const GameString& _RenderPath);
	GameRenderData* CreateRenderData(const GameString& _MeshName, const GameString& _RenderPath, CTransData& _TransData);
	//void ChangeRenderData(const GameString& _RDName);
};

