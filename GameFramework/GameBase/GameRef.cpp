#include "GameRef.h"


GameRef::GameRef() : m_REFCOUNT(0), m_bDebug(false)
{
}


GameRef::~GameRef()
{
}


void GameRef::PlusRef()
{
	// ���� �����ϴ� ����Ʈ ������ ��
	// std::shared_ptr<>
	++this->m_REFCOUNT;
}

void GameRef::MinusRef()
{
	--this->m_REFCOUNT;

	if (0 >= m_REFCOUNT)
	{
		delete this;
	}
}
