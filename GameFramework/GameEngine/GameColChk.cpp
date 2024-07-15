#include "GameCol.h"
#include "GameActor.h"
#include "GameScene.h"
#include "GameColMgr.h"
#include <DirectXCollision.h>
#include <DirectXCollision.inl>



void GameCol::COLAABBBOX::DataUpdate(GameTransform* _DataSetting)
{
	DATA.Center = _DataSetting->WPOS();
	DATA.Extents = _DataSetting->WSCALE().ABS().HV();
}

bool GameCol::COLAABBBOX::ColCheck(COLDATA* _Other)
{
	switch (_Other->Type)
	{
	case GameCol::SCOLTYPE::AABBRECT:
		return DATA.Intersects(((COLAABBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::OBBRECT:
		return DATA.Intersects(((COLOBBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::CIRCLE:
		return DATA.Intersects(((COLCIRCLE*)_Other)->DATA);
	default:
		break;
	}

	return false;
}

void GameCol::COLOBBBOX::DataUpdate(GameTransform* _DataSetting)
{
	DATA.Center = _DataSetting->WPOS();
	DATA.Extents = _DataSetting->WSCALE().ABS().HV();
	DATA.Orientation = _DataSetting->WQ();
}

bool GameCol::COLOBBBOX::ColCheck(COLDATA* _Other)
{
	switch (_Other->Type)
	{
	case GameCol::SCOLTYPE::AABBRECT:
		return DATA.Intersects(((COLAABBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::OBBRECT:
		return DATA.Intersects(((COLOBBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::CIRCLE:
		return DATA.Intersects(((COLCIRCLE*)_Other)->DATA);
	default:
		break;
	}

	return false;
}

void GameCol::COLCIRCLE::DataUpdate(GameTransform* _DataSetting)
{
	DATA.Center = _DataSetting->WPOS();
	DATA.Radius = _DataSetting->WSCALE().ABS().HV().x;
}

bool GameCol::COLCIRCLE::ColCheck(COLDATA* _Other)
{
	switch (_Other->Type)
	{
	case GameCol::SCOLTYPE::AABBRECT:
		return DATA.Intersects(((COLAABBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::OBBRECT:
		return DATA.Intersects(((COLOBBBOX*)_Other)->DATA);
	case GameCol::SCOLTYPE::CIRCLE:
		return DATA.Intersects(((COLCIRCLE*)_Other)->DATA);
	default:
		break;
	}

	return false;
}