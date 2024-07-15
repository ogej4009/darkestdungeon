#include "GameWin.h"
#include "GameInput.h"
#include "GameTime.h"
#include "GameMath.h"
#include "GameSound.h"

std::map<std::wstring, WNDCLASSEXW> GameWin::m_ClassMap; //RegClass ������(�̸�)
std::map<std::wstring, SPtr<GameWin> > GameWin::m_WinMap;  //GameWin Ŭ���� ������(�̸�)
std::map<HWND, SPtr<GameWin> > GameWin::m_HWndMap;     // Ŭ���� ������(�������ڵ�)
SPtr<GameWin> GameWin::m_pMainWindow = nullptr;
HDC GameWin::m_MainDc;

//WinProc�� ���� �Լ� ������, ������ �޼����� ó��
LRESULT CALLBACK GameWin::MSGFUNC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroySGameWin(hWnd);

		// �����쿡 ��Ʈ���� �޼����� �߻���Ų��.
		// DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int GameWin::BaseLoop(void(WINAPI* _GameFunc)(), void(WINAPI * _InitFunc)() /*= nullptr*/)
{
	if (nullptr != _InitFunc)
	{
		_InitFunc();
	}

	MSG msg;
	while (0 != m_WinMap.size())
	{

		//GetMessage�� �޼����� ���� ������ ��� ����Ѵ�.
		if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_GameFunc();
			//�޼��� ����Ÿ��
			//������ �� �Լ� �ȿ��� �����ؼ� �� �Լ� �ȿ��� ������.
		}
	}
	return (int)msg.wParam;
}

GameWin::WININIT GameWin::Construct = GameWin::WININIT();

GameWin::WININIT::WININIT()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GameWin::MSGFUNC;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = 0;
	wcex.hIcon = 0;/*LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPI))*/;
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"DefaultWinClass";
	wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegWindowClass(wcex);
}




GameWin::WININIT::~WININIT() {

}

GameWin::GameWin(HWND _HWnd) : m_HWnd(_HWnd)
{
	m_DC = GetDC(_HWnd); //GetDC ������ �Լ�
}

GameWin::GameWin()
{
}

GameWin::~GameWin()
{
}

void GameWin::DestroySGameWin(HWND _HWnd)
{
	std::map<HWND, SPtr<GameWin> >::iterator FindIter = m_HWndMap.find(_HWnd);
	if (FindIter == m_HWndMap.end())
	{
		ASSERTMSG;
	}
	GameString temp = FindIter->second->Name();
	std::map<std::wstring, SPtr<GameWin> >::iterator NameFindIter = m_WinMap.find(FindIter->second->Name());
	if (NameFindIter == m_WinMap.end())
	{
		ASSERTMSG;
	}

	//delete FindIter->second; //GameWin ��ü ����,���� �������� ����� ���� �޼����� ȣ���ؾ� �Ѵ�.
	m_WinMap.erase(NameFindIter);
	m_HWndMap.erase(FindIter);

}


bool GameWin::IsRegClass(const wchar_t* _Name)
{
	std::map<std::wstring, WNDCLASSEXW>::iterator FindIter = m_ClassMap.find(_Name);
	if (FindIter == m_ClassMap.end())
	{
		return false;
	}
	return true;
}

void GameWin::RegWindowClass(const WNDCLASSEXW& _wcex)
{
	// �̹� ��ϵ� Ŭ�������� ��Ʈ��
	if (true == IsRegClass(_wcex.lpszClassName))
	{
		ASSERTMSG;
	}

	// ����� �����ص� ����.
	if (0 == RegisterClassExW(&_wcex))  //���� ��� �� ���� 
	{
		ASSERTMSG;
	}

	m_ClassMap.insert(std::map<std::wstring, WNDCLASSEXW>::value_type(_wcex.lpszClassName, _wcex));
}

SPtr<GameWin> GameWin::FindSGameWin(const wchar_t* _WinName)
{
	std::map<std::wstring, SPtr<GameWin>>::iterator FindIter = m_WinMap.find(_WinName);
	if (FindIter == m_WinMap.end())
	{
		return nullptr;
	}
	return FindIter->second;
}


SPtr<GameWin> GameWin::CreateSGameWin(const wchar_t* _WinName, const wchar_t* _ClassName /*= L"DefaultWinClass"*/, bool _bShow /*= true*/)
{
	if (nullptr != FindSGameWin(_WinName))
	{
		ASSERTMSG;
	}

	if (false == IsRegClass(_ClassName))
	{
		ASSERTMSG;
	}

	HWND hWnd = CreateWindowW(_ClassName, _WinName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, /*hInstance*/0, nullptr);

	if (0 == hWnd)
	{
		return FALSE;
	}

	GameWin* NewWin = new GameWin(hWnd); //��������
	NewWin->m_Style = WS_OVERLAPPEDWINDOW;
	NewWin->Name(_WinName);
	m_WinMap.insert(std::map<std::wstring, GameWin*>::value_type(_WinName, NewWin));
	m_HWndMap.insert(std::map<HWND, GameWin*>::value_type(hWnd, NewWin));

	if (1 == m_WinMap.size())
	{
		m_pMainWindow = NewWin;
		m_MainDc = m_pMainWindow->WINHDC(); //GetDc ������ �Լ��� HDC�������� �װ� ��ȯ
	}


	if (true == _bShow)
	{
		NewWin->Show();
	}

	return NewWin;
}

void GameWin::Show()
{
	ShowWindow(m_HWnd, SW_SHOW);
}

bool GameWin::Size(CVector _Size)
{
	m_Size = _Size;
	// ũ�Ⱑ -> �ٿ� �װͱ��� ��ģ ����� 800
	// �ɹ������� ��� PUBLIC�̸� �䷱������ ������ ����.
	RECT Rc = { 0 , 0, m_Size.ix(), m_Size.iy() };
	HMENU MenuCon = GetMenu(m_HWnd); // window �Լ� getMenu, �޴� �ڵ��� ����

	if (nullptr == MenuCon)
	{
		AdjustWindowRect(&Rc, m_Style, FALSE);
	}
	else {
		AdjustWindowRect(&Rc, m_Style, TRUE);
	}

	int x = Rc.right - Rc.left;
	int y = Rc.bottom - Rc.top;

	return SetWindowPos(m_HWnd, 0, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);

}




bool GameWin::TryReSizeWindow(const int& _Width, const int& _Height)
{
	CVector TempVec = { (float)_Width, (float)_Height };
	return Size(TempVec);
}

//���� ������ �������� �������� ���콺 ��ġ ����
CVector GameWin::GetMousePos(const wchar_t* _WinName)
{
	POINT p = { 0,0 };
	CVector mousePos = { 0,0 };
	if (GetCursorPos(&p))
	{
		SPtr<GameWin> TEMP = GameWin::FindSGameWin(_WinName);
		ScreenToClient(TEMP->GetHWND(), &p);
		mousePos = { (float)p.x, (float)p.y };
	}
	else
	{
		ASSERTMSG;
	}

	return mousePos;
}

 SPtr<GameWin> GameWin::AddSGameWin(const wchar_t* _WinName, HWND _WinHWnd)
{
	 if (nullptr != FindSGameWin(_WinName))
	 {
		 ASSERTMSG;
	 }

	 // WS_OVERLAPPEDWINDOW Ÿ��Ʋ�� ���� ��Ÿ�� �ɼ��� �־���.
	 HWND hWnd = _WinHWnd;

	 if (0 == hWnd)
	 {
		 return FALSE;
	 }

	 GameWin* NewWin = new GameWin(hWnd);
	 NewWin->m_Style = WS_OVERLAPPEDWINDOW;
	 NewWin->Name(_WinName);

	 //ũ�� �������� �ϴ� �κ�
	 RECT RC;
	 GetWindowRect(_WinHWnd, &RC);
	 NewWin->m_Size.x = (float)(RC.right - RC.left);
	 NewWin->m_Size.y = (float)(RC.bottom - RC.top);

	 m_WinMap.insert(std::map<std::wstring, GameWin*>::value_type(_WinName, NewWin));
	 m_HWndMap.insert(std::map<HWND, GameWin*>::value_type(hWnd, NewWin));

	 if (1 == m_WinMap.size())
	 {
		 m_pMainWindow = NewWin;
		 m_MainDc = m_pMainWindow->WINHDC();
	 }

	 return NewWin;
}

 CVector GameWin::MouseDir3D()
 {
	 return m_CurMousePos3D - m_PrevMousePos3D;
 }

 CVector GameWin::MousePos3D()
 {
	 CVector MPos = MousePos();
	 MPos.x -= Size().HV().x;
	 MPos.y -= Size().HV().y;
	 MPos.y *= -1.0f;

	 m_PrevMousePos3D = m_CurMousePos3D;
	 m_CurMousePos3D = MPos;

	 return m_CurMousePos3D;
 }

 void GameWin::Update()
 {
	 MousePos3D();
 }

 void GameWin::AllWinUpdate()
 {
	 for (auto& Window : m_WinMap)
	 {
		 Window.second->Update();
	 }
 }