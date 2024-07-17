#include "TargetRender.h"
#include <GameDic.h>
#include <GameTexture.h>
#include <GameAni.h>
#include <GameSprite.h>
#include <GameImage.h>
#include <GameJsonReader.h>
#include <GameRef.h>
#include <GameInput.h>
#include <GameCam.h>

SPtr<GameSprite> TargetRender::m_TargetBlood = nullptr;
std::vector<CVector> TargetRender::m_TargetBloodUVData;
int TargetRender::m_TargetBloodUVDataSize = 0;

TargetRender::TargetRender()
{

}

TargetRender::~TargetRender()
{

}

void TargetRender::Init()
{
	m_TargetActor[0] = CreateActor();
	m_TargetActor[0]->Trans()->WPOS({ 0,0,0,0 });
	m_TargetActor[1] = CreateActor();
	m_TargetActor[1]->Trans()->WPOS({ 0,0,0,0 });
	m_TargetActor[2] = CreateActor();
	m_TargetActor[2]->Trans()->WPOS({ 0,0,0,0 });
	m_TargetActor[3] = CreateActor();
	m_TargetActor[3]->Trans()->WPOS({ 0,0,0,0 });

	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	if (!m_Dir.TryFindFileRecursive(L"Target"))
	{
		ASSERTMSG;
	}

	//Actor 한개당 Ani 한개씩만 다룰 수 있다.
	if (nullptr == m_TargetBlood)
	{
		GameTexture::Load(m_Dir.CreateGameFilePath(L"blood_splatter_target.png"));
		m_TargetBlood = GameSprite::Create(m_Dir.CreateGameFilePath(L"blood_splatter_target.png"), 1, 1);
		SPtr<GameJsonReader> tempReader = Actor()->GetCom<GameJsonReader>();
		tempReader->ParseJson(m_Dir.CreateGameFilePath(L"blood_splatter_target.json"));
		m_TargetBloodUVData = tempReader->MakeUvData();
		m_TargetBlood->ChangeCutData(m_TargetBloodUVData);
		m_TargetBloodUVDataSize = (int)m_TargetBloodUVData.size();
	}

	for (int i = 0; i < 4; i++)
	{
		m_ImgRender[i] = m_TargetActor[i]->AddCom<GameImage>((unsigned int)RENDERGROUP::RG_TARGET);
		m_ImgRender[i]->WSCALE({ 4.0f, 4.0f });
		CVector tempPos = LogicHeader::MonsterCombatPos[i];
		tempPos.z -= 0.3f + (i * 0.1f);
		m_ImgRender[i]->WPOS(tempPos);
		m_Ani[i] = m_TargetActor[i]->AddCom<GameAni>();
	}

}

void TargetRender::HeroTargetInit(const GameString& _HeroName)
{
	GameDic m_Dir;
	m_Dir.MoveParent(L"GameFramework");
	if (!m_Dir.TryFindFileRecursive(L"Target"))
	{
		ASSERTMSG;
	}

	m_Dir.Move(L"Hero");
	m_Dir.Move(_HeroName);

	std::vector<GameFile> FSMlist = m_Dir.DirToAllDir();
	GameFile loadUvFile;
	std::vector<CVector> tempVec;
	GameString pngStr;
	GameString jsonStr;
	GameString fsmName;
	std::vector<GameFile> temp;
	SPtr<GameJsonReader> tempReader = Actor()->GetCom<GameJsonReader>();

	//언제 4개를 대상으로 할지 모르므로 각자를 다 돌면서 초기화를 해준다.
	//이미지 랜더러는 Ani와 연동될 한개만 있으면 된다.
	//2DAni가 스프라이트 이름을 가지고 알아서 랜더러에 세팅을 해줌

	for (int i = 0; i < 4; i++)
	{
		m_ImgRender[i]->WSCALE({ -4.0f, 4.0f });
	}

	for (auto& FSM : FSMlist)
	{
		m_Dir.Move(FSM.CFileName());
		temp = m_Dir.DirToAllFile();

		for (int i = 0; i < 4; i++)
		{
			fsmName = _HeroName.m_Str + L"_";
			tempVec.clear();

			//타겟 이미지가 없으면 blood로 세팅한다.
			if (1 > temp.size())
			{
				fsmName += FSM.CFileName();
				m_ImgRender[i]->Sprite(m_TargetBlood);
				m_Ani[i]->CreateAni(fsmName, L"blood_splatter_target.png", 0, m_TargetBloodUVDataSize - 1, 0.03f, true);
			}
			else
			{
				for (auto& item : temp)
				{
					if (item.Ext() == L".png")
					{
						pngStr = item.CFileName() + L".png";
					}

					if (item.Ext() == L".json")
					{
						jsonStr = item.CFileName() + L".json";
					}
				}

				if (nullptr == GameTexture::Find(pngStr))
				{
					GameTexture::Load(m_Dir.CreateGameFilePath(pngStr));
					GameSprite::Create(m_Dir.CreateGameFilePath(pngStr), 1, 1);
				}

				fsmName += FSM.CFileName();
				GameFile loadUvFile = m_Dir.CreateGameFileReadOnly(_HeroName.m_Str + L"uvData.data");

				if (true == loadUvFile.Open())
				{
					int uvCount = 0;
					loadUvFile.Read(uvCount);
					tempVec.reserve(uvCount);

					CVector tempSVEC4;
					for (int i = 0; i < uvCount; i++)
					{
						loadUvFile.Read(tempSVEC4);
						tempVec.push_back(tempSVEC4);
					}
					m_ImgRender[i]->SpriteUv(pngStr, tempVec);
					m_Ani[i]->CreateAni(fsmName, pngStr, 0, uvCount - 1, 0.04f, true);
					//fsm->InputFSMName(fsmName);
				}
				else
				{
					int cutCount = 1;

					tempReader->ParseJson(m_Dir.CreateGameFilePath(jsonStr));
					tempVec = tempReader->MakeUvData();
					m_ImgRender[i]->SpriteUv(pngStr, tempReader->MakeUvData());
					cutCount = (int)tempVec.size() - 1;

					GameFile saveUvFile = m_Dir.CreateGameFile(_HeroName.m_Str + L"uvData.data");
					saveUvFile.Open();
					saveUvFile.Write((int)tempVec.size());

					for (int j = 0; j < (int)tempVec.size(); j++)
					{
						saveUvFile.Write(tempVec[j]);
					}

					m_Ani[i]->CreateAni(fsmName, pngStr, 0, cutCount, 0.04f, true);
					//fsm->InputFSMName(fsmName);
				}
			}

			m_Ani[i]->ChangeAni(fsmName);
			m_ImgRender[i]->Off();
		}
		m_Dir.MoveParent();
	}
}

void TargetRender::TargetRenderOn(int _SKillNum, SPtr<GameCam> _CAM, std::vector<bool> _Target)
{
	GameString SkillName;
	SkillName.m_Str.reserve(50);
	SkillName = Actor()->SStringName() + L"_" + ActorList::TargetSkillName[_SKillNum];
	CVector tempPos;

	if (m_Ani[0]->m_AllAni.size() < _SKillNum)
	{
		ASSERTMSG;
	}

	m_TargetRenderTimer = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		if (_Target[i] == false)
		{
			continue;
		}

		m_ImgRender[i]->On();
		tempPos = LogicHeader::MonsterCombatPos[i];
		tempPos += _CAM->Trans()->WPOS();
		tempPos.z -= 0.3f + (i * 0.1f);
		m_ImgRender[i]->WPOS(tempPos);
		m_Ani[i]->ChangeAni(SkillName);
	}
}

void TargetRender::Update()
{
	if (m_TargetRenderTimer > 0)
	{
		m_TargetRenderTimer -= GameTime::DeltaTime();
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			m_ImgRender[i]->Off();
		}
	}
}