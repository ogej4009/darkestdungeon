#pragma once
#include "afxdialogex.h"


// ObjMenuDlg 대화 상자

class ObjMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ObjMenuDlg)

public:
	ObjMenuDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ObjMenuDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjMenuDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
