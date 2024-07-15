#pragma once
#include <windows.h>
#include <map>
#include <string>
#include "GameMath.h"
#include "GameMacro.h"
#include "GameRef.h"
#include "GameName.h"

class GameWin : public GameRef, public GameName
{
	//////////�߻�Ŭ����
public:
	class LOOPUPDATER
	{
	public:
		virtual void GAMEUPDATE() = 0;
		virtual void GAMEINIT() = 0;
	};

////Regist before Create Class
private:
	class WININIT
	{
	public:
		WININIT();
		~WININIT();
	};

	static WININIT Construct;

/////////////////static ��� ����,�Լ���
private:
	static std::map<std::wstring, WNDCLASSEXW> m_ClassMap;  // Ŭ���� �̸����� Ŭ���� ��ü ã��
	static std::map<std::wstring, SPtr<GameWin> > m_WinMap; //������ �̸����� ��ü ã��
	static std::map<HWND, SPtr<GameWin> > m_HWndMap; //������ �ڵ�� Ŭ���� ��ü ã��
	static SPtr<GameWin> m_pMainWindow;
	static HDC m_MainDc;

public: //get set
	static SPtr<GameWin> MainWindow()
	{
		return m_pMainWindow;
	}

	static HDC MainHDC()
	{
		return m_MainDc;
	}

private:
	static void DestroySGameWin(HWND _HWnd);

public:
	static LRESULT __stdcall MSGFUNC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static bool IsRegClass(const wchar_t* _ClassName);
	static void RegWindowClass(const WNDCLASSEXW& _wcex);
	static SPtr<GameWin> FindSGameWin(const wchar_t* _WinName);
	static SPtr<GameWin> CreateSGameWin(const wchar_t* _WinName, const wchar_t* _ClassName = L"DefaultWinClass", bool _bShow = true);
	static SPtr<GameWin> AddSGameWin(const wchar_t* _WinName, HWND _WinHWnd);
	static int BaseLoop(void(WINAPI * _GameFunc)(), void(WINAPI * _InitFunc)() = nullptr);
	static CVector GetMousePos(const wchar_t* _WinName);
	static void AllWinUpdate();

public:
	bool TryReSizeWindow(const int& _width,const int& _height);
	
	CVector MousePos()
	{
		POINT Point;

		GetCursorPos(&Point);
		ScreenToClient(m_HWnd, &Point);

		CVector ReturnPos;

		ReturnPos.x = (float)Point.x;
		ReturnPos.y = (float)Point.y;

		return ReturnPos;
	}

	CVector MousePos3D();
	CVector MouseDir3D();

private: ////member
	int m_Style;
	HWND m_HWnd; // ������ ���� ����
	HDC m_DC; //������ �׸��� �ִ� ����
	CVector m_Size;
	CVector m_CurMousePos3D;
	CVector m_PrevMousePos3D;

public:
	HDC WINHDC() //��� HDC����
	{
		return m_DC;
	}

	HWND GetHWND()
	{
		return m_HWnd;
	}


public:
	void Show();
	void Update();

	bool Size(CVector _Size);
	CVector Size() { return m_Size; }
	CVector SizeHalf() { return m_Size.HV(); }

private:
	GameWin(HWND _HWnd);
	GameWin();

private:
	virtual ~GameWin();
};

