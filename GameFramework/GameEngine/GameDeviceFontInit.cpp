#include "GameDevice.h"
#include "GameFont.h"

void GameDevice::FontInit() 
{
	{
		//����׷����� ù��° ��Ʈ����Ʈ���� ������ ������ �����س���
		GameFont::Create(L"ȣ��ü");
		GameFont::m_FontList.push_back(L"ȣ��ü");
	}
}