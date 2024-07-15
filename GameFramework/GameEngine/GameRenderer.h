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


//렌더링 대상들이 가지고 있을 랜더링 정보

class GameDebugEn;
class GameRenderData : public GameObjRef
{
private:
	class RENDERRES
	{
	public:
		unsigned int Slot;
	};

	//RenderPath가 가지고 있을 정보들
	//클래스화 시켰음, CB,TEX,SMP
	//이름은 오늘 가면 추가할듯 지금은 res에 Slot번호만 입력해놨음
	class CBDATA : public RENDERRES
	{
	public:
		SPtr<GameConstBuffer> CB;
		unsigned int Size;
		//직접 링크로 동작을 할건가?
		bool m_IsLink;
		//자기만의 어떤 컬러, 트랜스 데이터 등등
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

	//랜더러가 부모가 되고 여러 매쉬와 랜더패스를 가지고 있을 수 있다.
	//자신의 Update()에서 Render()를 진행해서 각자의 랜더링 옵션을 세팅해준다.
private:
	//부모 정보를 포인터로 들고 있었는데 디버그 랜더링할때 동일한 방식으로
	//진행하기 위해 뺏다. 대신 TransData만 들고 있도록 바뀜
	//GameRenderer* m_pParent;
	CTransData* m_pData;
	SPtr<GameMesh> m_Mesh;
	SPtr<GameRenderMaker> m_RenderPath;
	//삼각형으로 그려줘 이런 모드를 가지고 있을 변수
	D3D11_PRIMITIVE_TOPOLOGY m_DrawMode;

private:
	//텍스처 자원을 가지고 있을 자료구조.
	//쉐이더 타입과 이름을 들고 있음
	std::map<GameShader::SHADER_TYPE, std::map<GameString, TEXDATA>> TexRes;
	std::map<GameShader::SHADER_TYPE, std::map<GameString, CBDATA>> CbRes;
	std::map<GameShader::SHADER_TYPE, std::map<GameString, SMPDATA>> SmpRes;

public:
	GameRenderData(const GameString& _Mesh, const GameString& _Path);
	~GameRenderData();

public:

	//RenderData에서 CB세팅, 이름, 데이터 쉐이더타입을 받는다.
	//단 쉐이더 타입은 기본 인자값을 가지고 있어서 입력시 특정 쉐이더 타입으로
	// 미입력시 모든 쉐이더를 돌면서 찾는다.

	//CB의 경우 Link모드와 비Link모드가 존재하는데
	//Link모드는 _Data의 포인터 형태로 동작하고 아닌건 객체 내부에 동적할당을 가리키는 포인터가 생긴다고 보면 된다.
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

