#pragma once
#include <..\>
#include <map>
#include <list>
#include "GameFile.h"

//dll�� ��� 32��Ʈ 64��Ʈ ������ ����� ��� �����ؾ� �ϳ�?
class GameSound
{

public:
	class GameSoundStream
	{
	private:
		friend GameSound;
		FMOD::Channel* m_Ch;
		bool m_bActive;

	private:
		void Init(FMOD::Channel* _Ch);

	public:
		void Stop();
		void Paused();
		void ReStart();
		void Position(unsigned int _Time);
		void PlayOnOff();
		void ReleaseStream();

	private:
		GameSoundStream();
		~GameSoundStream();
	};

	friend GameSoundStream;

private:
	// FMOD�� 
	static FMOD::System* m_pSoundSystem;

public:
	static std::map<std::wstring, GameSound*> m_AllSound;
	static std::list<GameSoundStream*> m_AllStream;
	static std::list<GameSoundStream*> m_ActiveStream;

private:
	static GameSoundStream* PopSoundStream();

public:
	static void SoundInit(int count= 1024);
	static void SoundEnd();
	static void SoundUpdate(); // �̺�Ʈ üũ��
	static void SoundDestroy(); // �޸� ����
	static void SoundLoad(const GameFile& _File); // �޸� ����

	// 0������ �����ϴ�. ����������.
	static GameSoundStream* SoundPlay(const wchar_t* _Name, int _Loop = 1); // �޸� ����

	/////////////////////// ���⼭���ʹ�

private:
	FMOD::Sound* m_pSound;

private:
	bool Load(const wchar_t* _Path);

public:
	GameSound();
	~GameSound();
};
