#pragma once
#include "GameMacro.h"
#include "GameTFunc.h"
#include "GameString.h"
//순환참조 주의하시길
// 침습형 메모리 관리
// GameRef 아래쪽에선 SPTR을 활용한 오퍼레이터 오버로딩이 되어있다.

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

	// 디버그를 잡고싶은 클래스를 대상으로 DebugOn()을 키고
	// FindIter->second->DebugCheck();
	// 이런 형태로 디버그 체크를 넣고 싶은 위치에 포함시켜서 디버그를 할 수 있다.
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

	//SPTR을 활용한 다운캐스팅에 사용되는 오퍼레이터다 
	//SCOM에서 STRANS로 넘어가는 경우 사용됨 
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

// nullptr_t nullptr의 자료형
//GameRef 아래쪽에선 SPTR을 활용한 오퍼레이터 오버로딩이 되어있다.
template<typename T>
bool operator==(std::nullptr_t _Left, const SPtr<T>& _Right)
{
	return _Right.operator==(_Left);
}