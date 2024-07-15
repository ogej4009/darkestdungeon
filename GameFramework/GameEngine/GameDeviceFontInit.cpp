#include "GameDevice.h"
#include "GameFont.h"

void GameDevice::FontInit() 
{
	{
		//디버그랜더는 첫번째 폰트리스트값을 가지고 가도록 세팅해놨음
		GameFont::Create(L"호국체");
		GameFont::m_FontList.push_back(L"호국체");
	}
}