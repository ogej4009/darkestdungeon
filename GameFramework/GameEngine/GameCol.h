#pragma once
#include "GameTransform.h"
#include <unordered_map>
#include <functional>
#include <unordered_set>

class GameColMgr;
class GameCol : public GameTransform
{
public:
	friend GameColMgr;

	// AABBȸ�� ���ϴ� �簢, OBBȸ���ϴ� �簢��
	enum class SCOLTYPE
	{
		AABBRECT,
		OBBRECT,
		CIRCLE,
	};

	class COLDATA : public GameRef
	{
	public:
		SCOLTYPE Type;

	public:
		virtual void DataUpdate(GameTransform* _DataSetting) = 0;
		virtual bool ColCheck(COLDATA* _Other) = 0;
	};

	class COLAABBBOX : public COLDATA
	{
	public:
		DirectX::BoundingBox DATA;
	public:
		void DataUpdate(GameTransform* _DataSetting) override;
		bool ColCheck(COLDATA* _Other) override;
	};

	class COLOBBBOX : public COLDATA
	{
	public:
		DirectX::BoundingOrientedBox DATA;
	public:
		void DataUpdate(GameTransform* _DataSetting) override;
		bool ColCheck(COLDATA* _Other) override;
	};

	class COLCIRCLE : public COLDATA
	{
	public:
		DirectX::BoundingSphere DATA;
	public:
		void DataUpdate(GameTransform* _DataSetting) override;
		bool ColCheck(COLDATA* _Other) override;
	};

private:
	int m_Order;
	SPtr<COLDATA> pData;
	//�浹 ����� ����ϰ� �ִٰ� ������ �����ִ� �뵵�� �������
	std::unordered_set<GameCol*> m_OtherCol;

	std::list<std::function<void(GameCol* _This, GameCol* _Other)>> m_EnterMemberEventList;
	std::list<std::function<void(GameCol* _This, GameCol* _Other)>> m_StayMemberEventList;
	std::list<std::function<void(GameCol* _This, GameCol* _Other)>> m_ExitMemberEventList;

public:
	GameCol();
	~GameCol();

public:
	void Type(SCOLTYPE _Type);

	virtual void Init(SCOLTYPE _Type, unsigned int _Order = 0);
	void Collision(GameCol* _Col);
	bool ColCheck(GameCol* _Col);
	void Release();
	void DebugRender() override;

	//�����Լ� �Ǵ� ����Լ��� �ݸ��� ��� �ΰ��� ���� ������
	//�ش� ������ �浹������ �߻��� �̺�Ʈ�� ���۽����ش�.
	template<typename TYPE>
	void AddEnterEvent(TYPE* _TypeObj, void(TYPE::*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _TypeObj || nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_EnterMemberEventList.push_back(std::bind(_Func, _TypeObj, std::placeholders::_1, std::placeholders::_2));
	}

	void AddEnterEvent(void(*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_EnterMemberEventList.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename TYPE>
	void AddStayEvent(TYPE* _TypeObj, void(TYPE::*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _TypeObj || nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_StayMemberEventList.push_back(std::bind(_Func, _TypeObj, std::placeholders::_1, std::placeholders::_2));
	}

	void AddStayEvent(void(*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_StayMemberEventList.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename TYPE>
	void AddExitEvent(TYPE* _TypeObj, void(TYPE::*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _TypeObj || nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_ExitMemberEventList.push_back(std::bind(_Func, _TypeObj, std::placeholders::_1, std::placeholders::_2));
	}

	void AddExitEvent(void(*_Func)(GameCol* _This, GameCol* _Other))
	{
		if (nullptr == _Func)
		{
			ASSERTMSG;
		}

		m_ExitMemberEventList.push_back(std::bind(_Func, std::placeholders::_1, std::placeholders::_2));
	}
};

