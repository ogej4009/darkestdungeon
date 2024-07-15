#include "GameRenderer.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameCam.h"
#include "GameConstBuffer.h"
/////////////////////RenderData

GameRenderData::GameRenderData(const GameString& _Mesh, const GameString& _Path) : m_DrawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	m_Mesh = GameMesh::Find(_Mesh);
	m_RenderPath = GameRenderMaker::Find(_Path);
}

GameRenderData::~GameRenderData()
{

}

void GameRenderData::RenderPathInit()
{
	for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
	{
		GameShader* SH = m_RenderPath->SHADER(i);

		if (nullptr == SH)
		{
			continue;
		}

		//쉐이더 타입에 따라 맵이 나뉘고
		//나뉜 쉐이더 타입에서 이름과 CBDATA로 또 구분이 된다.
		//랜더패스에서 세팅해놓은 정보들을 랜더데이타 구조체들이 가지도록
		//세팅하는 부분이다.
		for (auto& Res : SH->m_CB)
		{
			CBDATA CD = CBDATA();
			CD.Slot = Res.second->Slot();
			CD.CB = Res.second;

			//TRANSDATA는 랜더러가 무조건 들고있으니까 
			//꼭 링크로 처리해주자.
			if (true == CD.CB->Check<CTransData>())
			{
				CD.m_IsLink = true;
				CD.Size = sizeof(CTransData);
				CD.Data = m_pData;
			}

			//랜더패스의 이름과 랜더데이타에서 CB구조체를 저장해주렴
			CbRes[(GameShader::SHADER_TYPE)i].insert(std::map<GameString, CBDATA>::value_type(Res.first, CD));
		}

		for (auto& Res : SH->m_TEX)
		{
			TEXDATA TD = TEXDATA();
			TD.Slot = Res.second;
			TexRes[(GameShader::SHADER_TYPE)i].insert(std::map<GameString, TEXDATA>::value_type(Res.first, TD));
		}

		for (auto& Res : SH->m_SMP)
		{
			SMPDATA SD = SMPDATA();
			SD.Slot = Res.second;
			SmpRes[(GameShader::SHADER_TYPE)i].insert(std::map<GameString, SMPDATA>::value_type(Res.first, SD));
		}
	}
}

void GameRenderData::CbDataCreate(const GameString& _Name, void* _Data, bool _bLink /*= false*/, GameShader::SHADER_TYPE _Type/* = GameShader::SHADER_TYPE::ST_MAX*/)
{
	if (_Type != GameShader::SHADER_TYPE::ST_MAX)
	{
		//쉐이더 타입을 알고 링크로 동작을 할거니?
		//안다면 받아온 _Type정보를 그대로 사용한다.
		if (true == _bLink)
		{
			//기존에 쓰던중일지 모르니까 기존 데이터는 날려버리자.
			CbRes[_Type][_Name].Reset();
			//링크중이라고 세팅해주고
			CbRes[_Type][_Name].m_IsLink = _bLink;
			//크기를 입력해준다.
			CbRes[_Type][_Name].Size = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			CbRes[_Type][_Name].Data = _Data;
		}
		else
		{
			//비링크 타입으로 동작하고 쉐이더 종류도 모르는 경우
			unsigned int ByteSize = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			//데이터가 없다면 새로 크기만큼 char배열 형태로 동적할당 해줘라.
			if (nullptr == CbRes[_Type][_Name].Data)
			{
				CbRes[_Type][_Name].Data = new char[ByteSize];
				CbRes[_Type][_Name].Size = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			}
			//할당된 곳에 받아올 데이터의 메모리 카피를 해줘라.
			memcpy_s(CbRes[_Type][_Name].Data, ByteSize, _Data, ByteSize);
		}
	}
	else
	{
		//쉐이더 타입을 모른다. 전부 뺑뺑이 돌면서 찾는다.
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			//우리쪽 CbRes 맵에서 다 돌아
			//쉐이더중에 어떤 쉐이더를 가지고 있니?
			CBIter = CbRes.find((GameShader::SHADER_TYPE)i);

			if (CBIter == CbRes.end())
			{
				continue;
			}

			//쉐이더를 찾았다면 리소스 이름으로 검색을 해보렴
			CBFindIter = CBIter->second.find(_Name);

			if (CBFindIter == CBIter->second.end())
			{
				continue;
			}

			//쉐이더에 해당이름을 가진 리소스를 가지고 있고 링크모드로 동작한다면
			if (true == _bLink)
			{
				CBFindIter->second.Reset();
				CBFindIter->second.Size = CBFindIter->second.CB->BufferDesc().ByteWidth;
				CBFindIter->second.m_IsLink = _bLink;
				CBFindIter->second.Data = _Data;
			}
			//링크모드가 아니라면 메모리 카피
			else
			{
				unsigned int ByteSize = CBFindIter->second.CB->BufferDesc().ByteWidth;
				if (nullptr == CBFindIter->second.Data)
				{
					CBFindIter->second.Size = ByteSize;
					CBFindIter->second.Data = new char[ByteSize];
				}

				memcpy_s(CBFindIter->second.Data, ByteSize, _Data, ByteSize);

			}

		}

	}
}


void GameRenderData::SMP(const GameString& _Name, const GameString& _ResName, GameShader::SHADER_TYPE _Type/* = GameShader::SHADER_TYPE::ST_MAX*/)
{
	GameSampler* RES = GameSampler::Find(_ResName);

	if (nullptr == RES)
	{
		ASSERTMSG;
	}

	//타입이 정해졌다면 그걸 그대로 해당 쉐이더의 샘플러 리소스로 해주렴
	if (_Type != GameShader::SHADER_TYPE::ST_MAX)
	{
		SmpRes[_Type][_Name].Smp = RES;
	}
	//아니라면 쉐이더부터 찾아주렴
	else
	{
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			SmpIter = SmpRes.find((GameShader::SHADER_TYPE)i);

			if (SmpIter == SmpRes.end())
			{
				continue;
			}

			//쉐이더를 찾았다면 해당 샘플러 이름을 가지고 있는지 보렴
			SmpFindIter = SmpIter->second.find(_Name);

			if (SmpFindIter == SmpIter->second.end())
			{
				continue;
			}

			SmpFindIter->second.Smp = RES;

		}
	}
}

void GameRenderData::TEX(const GameString& _Name, const GameString& _ResName, GameShader::SHADER_TYPE _Type/* = GameShader::SHADER_TYPE::ST_MAX*/)
{
	GameTexture* RES = GameTexture::Find(_ResName);

	if (nullptr == RES)
	{
		ASSERTMSG;
	}

	TEX(_Name, RES, _Type);
}


void GameRenderData::TEX(const GameString& _Name, SPtr<GameTexture> _Tex, GameShader::SHADER_TYPE _Type/* = GameShader::SHADER_TYPE::ST_MAX*/)
{
	GameTexture* Res = _Tex;

	if (nullptr == Res)
	{
		ASSERTMSG;
	}

	if (_Type != GameShader::SHADER_TYPE::ST_MAX)
	{
		//키 두개를 인덱싱 형태로 입력할 수 있다.
		//첫번째 키는 쉐이더 타입, 두번째 키는 이름
		TexRes[_Type][_Name].Tex = Res;
	}
	else
	{
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			TexIter = TexRes.find((GameShader::SHADER_TYPE)i);

			//쉐이더 타입으로 먼저 한번 찾아봐라
			if (TexIter == TexRes.end())
			{
				continue;
			}

			//있으면 이름으로 검색해봐라.
			TexFindIter = TexIter->second.find(_Name);

			if (TexFindIter == TexIter->second.end())
			{
				continue;
			}

			//있다면 해당 이름의 리소스는 이거임
			TexFindIter->second.Tex = Res;
		}
	}

}

void GameRenderData::Render()
{
	m_Mesh->SettingVB();
	m_Mesh->SettingIB();
	m_RenderPath->Setting();

	GameDevice::MainContext()->IASetPrimitiveTopology(m_DrawMode);

	for (auto& Res : CbRes)
	{
		for (auto& CbData : Res.second)
		{
			if (nullptr == CbData.second.CB)
			{
				ASSERTMSG;
			}

			CbData.second.CB->SettingData(CbData.second.Data, CbData.second.Size);
			CbData.second.CB->Update();
		}
	}


	for (auto& Res : TexRes)
	{
		for (auto& TexData : Res.second)
		{
			if (nullptr == TexData.second.Tex)
			{
				ASSERTMSG;
			}

			TexData.second.Tex->SettingTex(Res.first, TexData.second.Slot);
		}
	}

	for (auto& Res : SmpRes)
	{
		for (auto& SmpData : Res.second)
		{
			if (nullptr == SmpData.second.Smp)
			{
				ASSERTMSG;
			}

			// Var.first 버텍스 쉐이더 픽셀 쉐이더 의
			// TexData.second.Slot 번째 리소스다.
			SmpData.second.Smp->Setting(Res.first, SmpData.second.Slot);
		}
	}

	m_Mesh->RenderIndexed(0);
}



/////////////////////Render///////////////////

GameRenderer::GameRenderer() : m_Order(0)
{
}


GameRenderer::~GameRenderer()
{
}

//이걸 호출하면 부모 액터의 장면의 RenderMGR을 호출해서 넣어줌
//DrawOrder도 세팅
void GameRenderer::Init(unsigned int _Order /*= 0*/)
{
	m_Order = _Order;

	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	//처음 생성하면서 부모의 Trans를 넣어줌
	Parent(Actor()->Trans());
	//랜더 매니저에 현재 랜더를 넣어줌
	Actor()->Scene()->RENDERMGR.PushRender(this);
}

void GameRenderer::SceneComInit(unsigned int _Order /*= 0*/)
{
	m_Order = _Order;
}

//랜더패스의 변경이 있을수도 있는 경우 자신을 클론해서
//복사본을 전달해준다.
//바꿀 부분은 추후 구현하겠지
SPtr<GameRenderMaker> GameRenderData::RenderPath()
{
	if (false == m_RenderPath->IsClone())
	{
		m_RenderPath = m_RenderPath->Clone();
	}

	return m_RenderPath;
}

void GameRenderer::Render(SPtr<GameCam> _Cam)
{
	GameTransform::Update();
	GameTransform::CalTransData(_Cam);

	//내 매쉬와 랜더패스를 가지고 있으면
	//그 매쉬를 랜더패스 내용대로 그려준다.

	for (auto& RD : m_RenderList)
	{
		RD->Render();
	}


	//아래의 내용은 RenderPath에서 해줘야 한다.
	//m_TransData.W = CMatrix::LaplaceMat3D;
	//SPtr<GameMesh> Mesh = GameMesh::Find(L"Rect");
	//Mesh->SettingVB();
	//Mesh->SettingIB();
	//
	//CMatrixData NewMat;
	//NewMat.Mat = CMatrix::LaplaceMat3D;
	//SPtr<GamePixShader> PIXSH = GamePixShader::Find(L"TexShader.fx");
	//PIXSH->Setting();
	//PIXSH->SettingData(0, NewMat);
	//PIXSH->SettingData(1, CVector::WHITE);

	//PIXSH->SettingCB();

	//

	////SPtr<GameTexture> Tex = GameTexture::Find(L"ShovelKnight.png");
	//SPtr<GameTexture> Tex = GameTexture::Find(L"blood_splatter.sprite.png");
	//Tex->SettingTex(GameShader::SHADER_TYPE::ST_PS, 0);

	//SPtr<GameSampler> LSmp = GameSampler::Find(L"LSMP");
	//SPtr<GameSampler> PSmp = GameSampler::Find(L"PSMP");

	//LSmp->Setting(GameShader::SHADER_TYPE::ST_PS, 0);
	//PSmp->Setting(GameShader::SHADER_TYPE::ST_PS, 1);


	//SPtr<GameBlend> BlendState = GameBlend::Find(L"DEFBLEND");
	//BlendState->Setting();


	//SPtr<GameVtxShader> VTXSH = GameVtxShader::Find(L"TexShader.fx");
	//VTXSH->Setting();
	//VTXSH->SettingData(0, m_TransData);

	//CVector CutUv;
	//CutUv.x = 1.0f / 5.0F;
	//CutUv.y = 0.0f;
	//CutUv.z = 1.0f / 5.0F;
	//CutUv.w = 1.0f / 3.0F;

	//VTXSH->SettingData(1, CutUv);
	//VTXSH->SettingCB();

	//// 인덱스버퍼의 3단위로 면을 그려라.
	//GameDevice::MainContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Mesh->RenderIndexed(0);
}


void GameRenderer::RenderUpdate(SPtr<GameCam> _Cam)
{
	//일단은 가상함수 용도로 사용됨
}


GameRenderData* GameRenderer::CreateRenderData(const GameString& _MeshName, const GameString& _RenderPath)
{
	GameRenderData* RD = new GameRenderData(_MeshName, _RenderPath);

	if (nullptr == RD->m_Mesh)
	{
		ASSERTMSG;
	}

	if (nullptr == RD->m_RenderPath)
	{
		ASSERTMSG;
	}

	RD->m_pData = &m_TransData;
	RD->RenderPathInit();
	m_RenderList.push_back(RD);

	return m_RenderList[m_RenderList.size() - 1];
}


GameRenderData* GameRenderer::CreateRenderData(const GameString& _MeshName, const GameString& _RenderPath, CTransData& _TransData)
{
	GameRenderData* RD = new GameRenderData(_MeshName, _RenderPath);

	if (nullptr == RD->m_Mesh)
	{
		ASSERTMSG;
	}

	if (nullptr == RD->m_RenderPath)
	{
		ASSERTMSG;
	}

	RD->m_pData = &_TransData;
	RD->RenderPathInit();
	m_RenderList.push_back(RD);

	return m_RenderList[m_RenderList.size() - 1];
}


//
//void GameRenderer::ChangeRenderData(const GameString& _RDName)
//{
//	SPtr<GameRenderData> Find = GameTFunc::MapFindPointer(m_RenderList, _RDName);
//
//	RD = Find;
//}
