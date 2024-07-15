// RDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameTool.h"
#include "afxdialogex.h"
#include "RDlg.h"


// RDlg 대화 상자

IMPLEMENT_DYNAMIC(RDlg, CDialogEx)

RDlg::RDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RDlg, pParent)
{

}

RDlg::~RDlg()
{
}

void RDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RDlg, CDialogEx)
END_MESSAGE_MAP()


// RDlg 메시지 처리기
