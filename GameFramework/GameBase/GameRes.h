#pragma once
#include "GameString.h"
#include "GameName.h"
#include "GameRef.h"
#include "GameMacro.h"
#include <map>


// 사운드-이미지-쉐이더-리소스-깊이버퍼-랜더타겟 >> GameRes
// 이름으로 관리 
// 당연히 레퍼런스 카운트로 관리 
// 이걸 상속받는다.

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
		//템플릿 선언을 이쪽에서도 한번 해줘야 했음, 자료구조 관련 요구사항인건가?
		typename std::map<GameString, SPtr<T>>::iterator FindIter = m_ResMap.find(_Name);

		if (m_ResMap.end() == FindIter)
		{
			return nullptr;
		}

		return FindIter->second;

	}

protected:
	//T는 Name을 상속받은 클래스여야 한다
	//아니면 터짐
	//일단 템플릿일때는 해당 함수에 대한 구현을 체크하지 않고
	//실제 실행시에 해당 부분을 체크한다.
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

//템플릿 정적선언 방법
template<typename T>
std::map<GameString, SPtr<T>> GameRes<T>::m_ResMap;
