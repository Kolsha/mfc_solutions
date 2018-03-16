// CMyProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "FirstTask.h"
#include "CMyProgressBar.h"


// CMyProgressBar

IMPLEMENT_DYNAMIC(CMyProgressBar, CWnd)

CMyProgressBar::CMyProgressBar()
{

}

CMyProgressBar::~CMyProgressBar()
{
}


BEGIN_MESSAGE_MAP(CMyProgressBar, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyProgressBar message handlers




void CMyProgressBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	//dc.FillSolidRect
	CRect rect;
	
	GetWindowRect(&rect);
	ScreenToClient(&rect);


	dc.Rectangle(rect);

	rect.right *= pos;
	rect.right /= max_pos;
	CBrush blueBrush(RGB(0, 0, 255));
	dc.FillRect(rect, &blueBrush);

	
}
