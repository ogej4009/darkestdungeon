#pragma once
#include <GameRes.h>
#include <vector>
#include "GameShader.h"

class GameVtxShader : public GameShader, public GameRes<GameVtxShader>
{
	class SINPUTLAYOUT : public GameRef
	{
	private:
		friend GameVtxShader;
		GameVtxShader* m_pParent;
		UINT m_Offset;
		ID3D11InputLayout* m_LayOut;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ArrInputData;

	public:
		// const char* _SmtName POSITION
		void AddLayOut(
			const char* _SmtName,
			unsigned int _Index,
			DXGI_FORMAT _Format,
			unsigned int _inputSlot,
			unsigned int _IDSR = 0,
			D3D11_INPUT_CLASSIFICATION _InputClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
		);
		void EndLayOut(
			const char* _SmtName,
			unsigned int _Index,
			DXGI_FORMAT _Format,
			unsigned int _inputSlot,
			unsigned int _IDSR = 0,
			D3D11_INPUT_CLASSIFICATION _InputClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
		);

	public:
		SINPUTLAYOUT(GameVtxShader* _pParent) : m_pParent(_pParent), m_Offset(0), m_LayOut(nullptr) {}
		~SINPUTLAYOUT() { if (nullptr != m_LayOut) { m_LayOut->Release(); } }
	};

	friend SINPUTLAYOUT;


private:
	ID3D11VertexShader* m_pVTXPTR;

public:
	//인풋 어셈블러에서 사용하는 레이아웃 INPUT_ELEMENT_DESC에 속함
	SINPUTLAYOUT LayOut;

public:
	GameVtxShader();
	virtual ~GameVtxShader();

public:
	static SPtr<GameVtxShader> Load(const GameString& _Path, const GameString& _FuncName, unsigned int _VH, unsigned int _VL)
	{
		GameString Name = _Path.PathData().ArrFullFile;
		GameVtxShader* NewRes = CreateRes(Name);
		NewRes->m_Path = _Path;
		if (false == NewRes->Load(_VH, _VL, _FuncName)) { ASSERTMSG; }
		return NewRes;
	}
	
	//따로 설정 건들지 않을 친구들은 여기서 처리해줌
	bool Load(unsigned int _VH, unsigned int _VL, const GameString& _FuncName)
	{
		D3D_SHADER_MACRO _MAC;
		_MAC.Definition = nullptr;
		_MAC.Name = nullptr;
		return Load(_VH, _VL, _FuncName, _MAC, nullptr);
	}

	bool Load(unsigned int _VH, unsigned int _VL, const GameString& _FuncName, D3D_SHADER_MACRO _MAC, ID3DInclude* _Inc);

	void Setting() override;
};

