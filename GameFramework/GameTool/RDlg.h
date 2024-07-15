#pragma once
#include "afxdialogex.h"


// RDlg 대화 상자

class RDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RDlg)

public:
	RDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
