#pragma once
#include "GameString.h"
#include "GameName.h"
#include "GameRef.h"
#include "GameMacro.h"
#include <map>


// ����-�̹���-���̴�-���ҽ�-���̹���-����Ÿ�� >> GameRes
// �̸����� ���� 
// �翬�� ���۷��� ī��Ʈ�� ���� 
// �̰� ��ӹ޴´�.

template<typename T>
class GameRes : public GameName, public GameRef
{

private:
	static std::map<GameString, SPtr<T>> m_ResMap;
	GameString m_Path;

protected:
	GameRes() {}
	virtual ~GameRes() {}

public:
	friend T;

	GameString Path() 
	{
		return m_Path;
	}

	static SPtr<T> Find(const wchar_t* _Name)
	{
		//���ø� ������ ���ʿ����� �ѹ� ����� ����, �ڷᱸ�� ���� �䱸�����ΰǰ�?
		typename std::map<GameString, SPtr<T>>::iterator FindIter = m_ResMap.find(_Name);

		if (m_ResMap.end() == FindIter)
		{
			return nullptr;
		}

		return FindIter->second;

	}

protected:
	//T�� Name�� ��ӹ��� Ŭ�������� �Ѵ�
	//�ƴϸ� ����
	//�ϴ� ���ø��϶��� �ش� �Լ��� ���� ������ üũ���� �ʰ�
	//���� ����ÿ� �ش� �κ��� üũ�Ѵ�.
	static void AddRes(const GameString& _Name, T* _Res)
	{
		_Res->Name(_Name);
		AddRes(_Res);
	}

	static void AddRes(T* _Res)
	{
		if (nullptr != Find(_Res->SStringName()))
		{
			ASSERTMSG;
		}

		m_ResMap.insert(std::map<GameString, SPtr<T>>::value_type(_Res->SStringName(), _Res));
	}

	static T* CreateRes(const GameString& _Name)
	{
		T* NewRes = new T();
		NewRes->Name(_Name);
		AddRes(NewRes);

		return NewRes;
	}

};

//���ø� �������� ���
template<typename T>
std::map<GameString, SPtr<T>> GameRes<T>::m_ResMap;
