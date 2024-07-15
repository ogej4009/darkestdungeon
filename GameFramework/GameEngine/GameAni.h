#pragma once
#include "GameLogic.h"
#include <functional>

class GameImage;
class GameAni : public GameLogic
{
	//클래스가 너무 커지는 경우 기능을 내부 이너클래스로 좀 나누는 형태로
	//간단하게 하는 방법이 있다 C#의 partial이라고 보면 될듯
private:
	class S2DANIDATA : public GameRef
	{
	public:
		friend GameAni;

	public:
		GameLogic* m_Parent;
		bool m_bLoop;
		bool m_bEnd;
		GameString m_SpriteName;
		GameString m_AniName;
		size_t m_Start;
		size_t m_End;
		size_t m_Cur;
		float m_InterTime;
		float m_CurTime;
		
		//콜백구조를 사용하는 함수포인터, this콜 함수는 받을수 없다.
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::iterator FindStartEventIter;
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>> m_StartEventList;
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>> m_EndEventList;

		//콜백구조를 사용하면서 this콜 함수도 functional기능을 활용해서 받아준다.
		std::map<size_t, std::list<std::function<void()>>>::iterator FindMemberStartEventIter;
		std::map<size_t, std::list<std::function<void()>>> m_StartMemberEventList;
		std::map<size_t, std::list<std::function<void()>>> m_EndMemberEventList;

	public:
		S2DANIDATA();
		~S2DANIDATA();

	public:
		void Update();
		void Reset();
	};

public:
	std::map<GameString, SPtr<S2DANIDATA>> m_AllAni;
	S2DANIDATA* m_CurAni;
	SPtr<GameImage> m_ImgRender;

public:
	GameAni() {}
	~GameAni() {}

public:
	void CreateAni(const GameString& _AniName, const GameString& _SpriteName, size_t _Start, size_t _End, float _Time, bool _Loop);
	void ChangeAni(const GameString& _AniName, bool _Loop = true, size_t _StartIndex = -1);
	SPtr<S2DANIDATA> FindAni(const GameString& _AniName);
	
	//전역으로 선언된 함수들만 받을 수 있는 형태
	void AddStartEvent(const GameString& _AniName, size_t _Frame, void(*_Func)(GameLogic* _Parent));

	//functional 기능을 활용해서 멤버함수들을 해당 객체와 연결 상태로 실행시켜주는 형태
	//이름, 프레임, 객체, 함수포인터 형태로 매개변수를 받는다.
	template<typename TYPE>
	void AddStartMemberEvent(const GameString& _AniName, size_t _Frame, TYPE* _TypeObj, void(TYPE::*_Func)())
	{
		if (nullptr == _TypeObj || nullptr == _Func) 
		{
			ASSERTMSG;
		}

		S2DANIDATA* FAni = FindAni(_AniName);

		std::map<size_t, std::list<std::function<void()>>>::iterator FAniIter = FAni->m_StartMemberEventList.find(_Frame);
		
		if (FAni->m_StartMemberEventList.end() == FAniIter)
		{
			//해당 이름이 리스트에 없다면 해당 이름으로 하나 만들어 줘
			FAni->m_StartMemberEventList.insert(std::map<size_t, std::list<std::function<void()>>>::value_type(_Frame, std::list<std::function<void()>>()));
			//이젠 없을 수 없지
			FAniIter = FAni->m_StartMemberEventList.find(_Frame);
		}

		//funtional 형태의 사용 방법 
		//여러 종류의 객체를 받을 수 있지만 대신 bind 형태로 써야하며
		//매개변수마다 std::placeholder 를 추가해줘야 한다. 최대 20개 
		FAniIter->second.push_back(std::bind(_Func, _TypeObj/*, std::placeholders::_1*/));
	}


	/*template<typename TYPE>
	void AddStartEvent(const GameString& _AniName, size_t _Frame, TYPE* _TypeObj, void(TYPE::*_Func)(GameLogic* _Func))
	{
		if (nullptr == _TypeObj || nullptr == _Func)
		{
			ASSERTMSG;
		}

		S2DANIDATA* FAni = FindAni(_AniName);

		std::map<size_t, std::list<std::function<void(GameLogic* _Func)>>>::iterator FAniIter = FAni->m_StartEventList.find(_Frame);

		if (FAni->m_StartEventList.end() == FAniIter)
		{
			FAni->m_StartEventList.insert(std::map<size_t, std::list<std::function<void(GameLogic*)>>>::value_type(_Frame, std::list<std::function<void(GameLogic*)>>()));
			FAniIter = FAni->m_StartEventList.find(_Frame);
		}

		FAniIter->second.push_back(std::bind(_Func, _TypeObj, std::placeholders::_1));
	}*/

	bool IsCurAni(const GameString& _Name)
	{
		return m_CurAni->m_AniName == _Name;
	}

	bool IsCurAniEnd()
	{
		return m_CurAni->m_bEnd;
	}

public:
	void Init() override;
	void UpdateNext() override;

};

