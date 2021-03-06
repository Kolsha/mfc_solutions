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
	CRect rect, tmp;
	
	GetWindowRect(&rect);
	ScreenToClient(&rect);


	dc.Rectangle(rect);

	if (!max_pos)
		max_pos++;

	
	//left_pos %= max_pos;
	//right_pos %= max_pos;

	tmp = rect;
	tmp.right *= double(right_pos) / double(max_pos);

	//rect.left += ((float(left_pos) / float(max_pos))) * rect.Width();

	CBrush blueBrush(RGB(0, 0, 255));
	dc.FillRect(tmp, &blueBrush);


	
	tmp.left = tmp.right;

	tmp.right = rect.right * (1 + (double(left_pos) / double(max_pos)));
	if (tmp.right > rect.right)
		tmp.right = rect.right;

	CBrush redBrush(RGB(255, 0, 0));
	dc.FillRect(tmp, &redBrush);
	

	
}
