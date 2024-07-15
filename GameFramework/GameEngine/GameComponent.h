#pragma once
#include <GameName.h>
#include <GameRTTI.h>
#include <list>
#include "GameActor.h"

//������Ʈ ������ ������Ʈ�� �ǹ��Ѵ�.
class GameTransform;
class GameComponent : public GameObjRef, public GameName, public GameRTTI
{
public:
	friend GameActor;

private:
	GameActor* m_pActor;


public:
	GameComponent();
	~GameComponent();

public:
	virtual void Init();

	//Scene�� Actor���� �Լ��� ȣ���ϰ� Actor�� ������Ʈ�� ������Ʈ�� ȣ���� 
	//Scene->Actor->Com ������ Update���ִ� �Լ���
	virtual void UpdatePrev() {}
	virtual void Update() {}
	virtual void UpdateNext() {}
	virtual void DebugRender() {}

public:
	GameActor* Actor()
	{
		return m_pActor;
	}

	//�⺻���� Actor�� �� ������Ʈ�� Trans�� �־��ֱ�� ����
	GameTransform* Trans();

	//�θ� �ڽİ����� �ٺ��� Actor�� Component�� �����̹Ƿ�
	//���⼭ ���� üũ�� �������̵� �Ѵ�.

	bool IsDeath() override
	{
		//������ ������ �����̰� ������ ������Ʈ�� �����̴�.
		//�θ� ���Ͱ� �׾��ٸ� ���� �����ŷ� �Ķ�
		return Actor()->IsDeath() || GameObjRef::IsDeath();
	}

	bool IsUpdate() override
	{
		//�θ� ������Ʈ �ؾ��ϰ� ���� ������Ʈ �ؾ��Ѵٸ� ������Ʈ ��
		return Actor()->IsUpdate() && GameObjRef::IsUpdate();
	}

	//�θ� On�̰� ���� On�̸� On�̾�
	bool IsOn() override
	{
		return Actor()->IsOn() && GameObjRef::IsOn();
	}

};
