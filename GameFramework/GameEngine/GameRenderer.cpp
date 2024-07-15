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

		//���̴� Ÿ�Կ� ���� ���� ������
		//���� ���̴� Ÿ�Կ��� �̸��� CBDATA�� �� ������ �ȴ�.
		//�����н����� �����س��� �������� ��������Ÿ ����ü���� ��������
		//�����ϴ� �κ��̴�.
		for (auto& Res : SH->m_CB)
		{
			CBDATA CD = CBDATA();
			CD.Slot = Res.second->Slot();
			CD.CB = Res.second;

			//TRANSDATA�� �������� ������ ��������ϱ� 
			//�� ��ũ�� ó��������.
			if (true == CD.CB->Check<CTransData>())
			{
				CD.m_IsLink = true;
				CD.Size = sizeof(CTransData);
				CD.Data = m_pData;
			}

			//�����н��� �̸��� ��������Ÿ���� CB����ü�� �������ַ�
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
		//���̴� Ÿ���� �˰� ��ũ�� ������ �ҰŴ�?
		//�ȴٸ� �޾ƿ� _Type������ �״�� ����Ѵ�.
		if (true == _bLink)
		{
			//������ ���������� �𸣴ϱ� ���� �����ʹ� ����������.
			CbRes[_Type][_Name].Reset();
			//��ũ���̶�� �������ְ�
			CbRes[_Type][_Name].m_IsLink = _bLink;
			//ũ�⸦ �Է����ش�.
			CbRes[_Type][_Name].Size = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			CbRes[_Type][_Name].Data = _Data;
		}
		else
		{
			//��ũ Ÿ������ �����ϰ� ���̴� ������ �𸣴� ���
			unsigned int ByteSize = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			//�����Ͱ� ���ٸ� ���� ũ�⸸ŭ char�迭 ���·� �����Ҵ� �����.
			if (nullptr == CbRes[_Type][_Name].Data)
			{
				CbRes[_Type][_Name].Data = new char[ByteSize];
				CbRes[_Type][_Name].Size = CbRes[_Type][_Name].CB->BufferDesc().ByteWidth;
			}
			//�Ҵ�� ���� �޾ƿ� �������� �޸� ī�Ǹ� �����.
			memcpy_s(CbRes[_Type][_Name].Data, ByteSize, _Data, ByteSize);
		}
	}
	else
	{
		//���̴� Ÿ���� �𸥴�. ���� ������ ���鼭 ã�´�.
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			//�츮�� CbRes �ʿ��� �� ����
			//���̴��߿� � ���̴��� ������ �ִ�?
			CBIter = CbRes.find((GameShader::SHADER_TYPE)i);

			if (CBIter == CbRes.end())
			{
				continue;
			}

			//���̴��� ã�Ҵٸ� ���ҽ� �̸����� �˻��� �غ���
			CBFindIter = CBIter->second.find(_Name);

			if (CBFindIter == CBIter->second.end())
			{
				continue;
			}

			//���̴��� �ش��̸��� ���� ���ҽ��� ������ �ְ� ��ũ���� �����Ѵٸ�
			if (true == _bLink)
			{
				CBFindIter->second.Reset();
				CBFindIter->second.Size = CBFindIter->second.CB->BufferDesc().ByteWidth;
				CBFindIter->second.m_IsLink = _bLink;
				CBFindIter->second.Data = _Data;
			}
			//��ũ��尡 �ƴ϶�� �޸� ī��
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

	//Ÿ���� �������ٸ� �װ� �״�� �ش� ���̴��� ���÷� ���ҽ��� ���ַ�
	if (_Type != GameShader::SHADER_TYPE::ST_MAX)
	{
		SmpRes[_Type][_Name].Smp = RES;
	}
	//�ƴ϶�� ���̴����� ã���ַ�
	else
	{
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			SmpIter = SmpRes.find((GameShader::SHADER_TYPE)i);

			if (SmpIter == SmpRes.end())
			{
				continue;
			}

			//���̴��� ã�Ҵٸ� �ش� ���÷� �̸��� ������ �ִ��� ����
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
		//Ű �ΰ��� �ε��� ���·� �Է��� �� �ִ�.
		//ù��° Ű�� ���̴� Ÿ��, �ι�° Ű�� �̸�
		TexRes[_Type][_Name].Tex = Res;
	}
	else
	{
		for (size_t i = 0; i < (size_t)GameShader::SHADER_TYPE::ST_MAX; ++i)
		{
			TexIter = TexRes.find((GameShader::SHADER_TYPE)i);

			//���̴� Ÿ������ ���� �ѹ� ã�ƺ���
			if (TexIter == TexRes.end())
			{
				continue;
			}

			//������ �̸����� �˻��غ���.
			TexFindIter = TexIter->second.find(_Name);

			if (TexFindIter == TexIter->second.end())
			{
				continue;
			}

			//�ִٸ� �ش� �̸��� ���ҽ��� �̰���
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

			// Var.first ���ؽ� ���̴� �ȼ� ���̴� ��
			// TexData.second.Slot ��° ���ҽ���.
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

//�̰� ȣ���ϸ� �θ� ������ ����� RenderMGR�� ȣ���ؼ� �־���
//DrawOrder�� ����
void GameRenderer::Init(unsigned int _Order /*= 0*/)
{
	m_Order = _Order;

	if (nullptr == Actor()->Scene())
	{
		ASSERTMSG;
	}

	//ó�� �����ϸ鼭 �θ��� Trans�� �־���
	Parent(Actor()->Trans());
	//���� �Ŵ����� ���� ������ �־���
	Actor()->Scene()->RENDERMGR.PushRender(this);
}

void GameRenderer::SceneComInit(unsigned int _Order /*= 0*/)
{
	m_Order = _Order;
}

//�����н��� ������ �������� �ִ� ��� �ڽ��� Ŭ���ؼ�
//���纻�� �������ش�.
//�ٲ� �κ��� ���� �����ϰ���
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

	//�� �Ž��� �����н��� ������ ������
	//�� �Ž��� �����н� ������ �׷��ش�.

	for (auto& RD : m_RenderList)
	{
		RD->Render();
	}


	//�Ʒ��� ������ RenderPath���� ����� �Ѵ�.
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

	//// �ε��������� 3������ ���� �׷���.
	//GameDevice::MainContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Mesh->RenderIndexed(0);
}


void GameRenderer::RenderUpdate(SPtr<GameCam> _Cam)
{
	//�ϴ��� �����Լ� �뵵�� ����
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
