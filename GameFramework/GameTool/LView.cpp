// LView.cpp: 구현 파일
//

#include "pch.h"
#include "GameTool.h"
#include "LView.h"


// LView

IMPLEMENT_DYNCREATE(LView, CFormView)

LView::LView()
	: CFormView(IDD_LView)
{

}

LView::~LView()
{
}

void LView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LView, CFormView)
END_MESSAGE_MAP()


// LView 진단

#ifdef _DEBUG
void LView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void LView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// LView 메시지 처리기
