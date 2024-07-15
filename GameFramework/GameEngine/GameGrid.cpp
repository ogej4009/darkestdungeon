#include "GameGrid.h"
#include "GameCam.h"

GameGrid::GameGrid()
{
	m_Data.Color = CVector::WHITE;
	m_Data.Step = 1.0f;
	m_Data.Border = 0.01f;
}

GameGrid::~GameGrid()
{

}


void GameGrid::Init(unsigned int _Order)
{
	GameRenderer::Init(_Order);
	RD = CreateRenderData(L"GRID", L"GRID");
	RD->CB(L"GRIDDATA", m_Data, true);
}

void GameGrid::RenderUpdate(SPtr<GameCam> _Cam)
{
	m_Data.CamPos = _Cam->Trans()->WPOS();
	m_Data.Step = 0.0;
	
	//y���� �����ͼ� �󸶳� ������ ���, ���ܰ����� �ٲ۴�.
	//���ܰ��� 1, 10, 100, 1000 �̷� ���·� �����ϰ� �Ѵ�.
	int CalY = (int)m_Data.CamPos.y;
	int CalYData = 0;

	while (CalY != 0)
	{
		++CalYData;
		CalY /= 10;
	}

	m_Data.Step = 1.0f;

	for (size_t i = 0; i < CalYData; i++)
	{
		m_Data.Step *= 10;
	}
}