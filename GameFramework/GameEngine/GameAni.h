#pragma once
#include "GameLogic.h"
#include <functional>

class GameImage;
class GameAni : public GameLogic
{
	//Ŭ������ �ʹ� Ŀ���� ��� ����� ���� �̳�Ŭ������ �� ������ ���·�
	//�����ϰ� �ϴ� ����� �ִ� C#�� partial�̶�� ���� �ɵ�
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
		
		//�ݹ鱸���� ����ϴ� �Լ�������, this�� �Լ��� ������ ����.
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>>::iterator FindStartEventIter;
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>> m_StartEventList;
		std::map<size_t, std::list<void(*)(GameLogic* _Parent)>> m_EndEventList;

		//�ݹ鱸���� ����ϸ鼭 this�� �Լ��� functional����� Ȱ���ؼ� �޾��ش�.
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
	
	//�������� ����� �Լ��鸸 ���� �� �ִ� ����
	void AddStartEvent(const GameString& _AniName, size_t _Frame, void(*_Func)(GameLogic* _Parent));

	//functional ����� Ȱ���ؼ� ����Լ����� �ش� ��ü�� ���� ���·� ��������ִ� ����
	//�̸�, ������, ��ü, �Լ������� ���·� �Ű������� �޴´�.
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
			//�ش� �̸��� ����Ʈ�� ���ٸ� �ش� �̸����� �ϳ� ����� ��
			FAni->m_StartMemberEventList.insert(std::map<size_t, std::list<std::function<void()>>>::value_type(_Frame, std::list<std::function<void()>>()));
			//���� ���� �� ����
			FAniIter = FAni->m_StartMemberEventList.find(_Frame);
		}

		//funtional ������ ��� ��� 
		//���� ������ ��ü�� ���� �� ������ ��� bind ���·� ����ϸ�
		//�Ű��������� std::placeholder �� �߰������ �Ѵ�. �ִ� 20�� 
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

