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
	//////////추상클래스
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

/////////////////static 멤버 변수,함수들
private:
	static std::map<std::wstring, WNDCLASSEXW> m_ClassMap;  // 클래스 이름으로 클래스 개체 찾기
	static std::map<std::wstring, SPtr<GameWin> > m_WinMap; //윈도우 이름으로 개체 찾기
	static std::map<HWND, SPtr<GameWin> > m_HWndMap; //윈도우 핸들로 클래스 개체 찾기
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
	HWND m_HWnd; // 윈도우 간섭 권한
	HDC m_DC; //윈도우 그릴수 있는 권한
	CVector m_Size;
	CVector m_CurMousePos3D;
	CVector m_PrevMousePos3D;

public:
	HDC WINHDC() //멤버 HDC리턴
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

