#pragma once
#include "afxdialogex.h"


// MapMenuDlg 대화 상자

class MapMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MapMenuDlg)

public:
	MapMenuDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MapMenuDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapMenuDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
