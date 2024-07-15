#pragma once
#include <GameRef.h>

//키고 끄고 죽어야 되는 오브젝트들을 관리하는 기능
class GameObjRef :	public GameRef
{
private:
	bool m_Active;
	bool m_Death;

public:
	GameObjRef();
	~GameObjRef();

public:
	virtual bool IsDeath()
	{
		return m_Death;
	}

	//지금은 랜더링 끄는거에만 쓰이고 있음
	virtual bool IsUpdate()
	{
		return true == m_Active && false == m_Death;
	}

	virtual bool IsOn()
	{
		return m_Active;
	}

	virtual void Death()
	{
		m_Death = true;
	}

	virtual void On() {
		m_Active = true;
	}

	virtual void Off() {
		m_Active = false;
	}

};

