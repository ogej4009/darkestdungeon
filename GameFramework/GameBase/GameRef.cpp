#include "GameRef.h"


GameRef::GameRef() : m_REFCOUNT(0), m_bDebug(false)
{
}


GameRef::~GameRef()
{
}


void GameRef::PlusRef()
{
	// 실제 제공하는 스마트 포인터 예
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
