#pragma once
#include "GameMacro.h"
#include "GameTFunc.h"
#include "GameString.h"
//��ȯ���� �����Ͻñ�
// ħ���� �޸� ����
// GameRef �Ʒ��ʿ��� SPTR�� Ȱ���� ���۷����� �����ε��� �Ǿ��ִ�.

class GameRef
{
public:
	template<typename T>
	friend class SPtr;

private:
	bool m_bDebug;
	int m_REFCOUNT;

public:
	void DebugOn() {
		m_bDebug = true;
	}

	void DebugOff() {
		m_bDebug = false;
	}

	// ����׸� ������ Ŭ������ ������� DebugOn()�� Ű��
	// FindIter->second->DebugCheck();
	// �̷� ���·� ����� üũ�� �ְ� ���� ��ġ�� ���Խ��Ѽ� ����׸� �� �� �ִ�.
	void DebugCheck()
	{
		if (false == m_bDebug)
		{
			return;
		}
		int a = 0;
	}

private:
	void PlusRef();
	void MinusRef();

public:
	GameRef();
	virtual ~GameRef();
};

template<typename T>
class SPtr
{
private:
	T* m_PTR;

public:
	template<typename T>
	operator T*()
	{
		return m_PTR;
	}

	//SPTR�� Ȱ���� �ٿ�ĳ���ÿ� ���Ǵ� ���۷����ʹ� 
	//SCOM���� STRANS�� �Ѿ�� ��� ���� 
	template<typename T>
	operator SPtr<T>()
	{
		return dynamic_cast<T*>(m_PTR);
	}


	T* operator->()
	{
		return m_PTR;
	}

	bool operator!=(const SPtr& _Other) const
	{
		return _Other.m_PTR != m_PTR;
	}


	bool operator==(std::nullptr_t _Other) const
	{
		return _Other == m_PTR;
	}

	SPtr& operator=(const SPtr& _Other)
	{
		if (nullptr == m_PTR)
		{
			m_PTR = _Other.m_PTR;
			m_PTR->PlusRef();
			return *this;
		}

		if (m_PTR == _Other.m_PTR)
		{
			return *this;
		}

		m_PTR->MinusRef();
		m_PTR = _Other.m_PTR;
		m_PTR->PlusRef();

		return *this;
	}

public:
	SPtr() : m_PTR(nullptr)
	{
	}

	SPtr(const SPtr& _PTR) : m_PTR(_PTR.m_PTR)
	{
		if (nullptr == m_PTR)
		{
			return;
		}

		this->m_PTR->PlusRef();
	}

	SPtr(T* _PTR) : m_PTR(_PTR)
	{
		if (nullptr == m_PTR)
		{
			return;
		}

		m_PTR->PlusRef();
	}

	~SPtr()
	{
		if (nullptr == m_PTR)
		{
			return;
		}

		m_PTR->MinusRef();
	}
};

template<typename T>
bool operator!=(std::nullptr_t  _Left, const SPtr<T>& _Right)
{
	return _Right.operator!=(_Left);
}

// nullptr_t nullptr�� �ڷ���
//GameRef �Ʒ��ʿ��� SPTR�� Ȱ���� ���۷����� �����ε��� �Ǿ��ִ�.
template<typename T>
bool operator==(std::nullptr_t _Left, const SPtr<T>& _Right)
{
	return _Right.operator==(_Left);
}