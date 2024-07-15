// ObjMenuDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameTool.h"
#include "afxdialogex.h"
#include "ObjMenuDlg.h"


// ObjMenuDlg 대화 상자

IMPLEMENT_DYNAMIC(ObjMenuDlg, CDialogEx)

ObjMenuDlg::ObjMenuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ObjMenuDlg, pParent)
{

}

ObjMenuDlg::~ObjMenuDlg()
{
}

void ObjMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ObjMenuDlg, CDialogEx)
END_MESSAGE_MAP()


// ObjMenuDlg 메시지 처리기
