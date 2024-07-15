// MapMenuDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameTool.h"
#include "afxdialogex.h"
#include "MapMenuDlg.h"


// MapMenuDlg 대화 상자

IMPLEMENT_DYNAMIC(MapMenuDlg, CDialogEx)

MapMenuDlg::MapMenuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MapMenuDlg, pParent)
{

}

MapMenuDlg::~MapMenuDlg()
{
}

void MapMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MapMenuDlg, CDialogEx)
END_MESSAGE_MAP()


// MapMenuDlg 메시지 처리기
