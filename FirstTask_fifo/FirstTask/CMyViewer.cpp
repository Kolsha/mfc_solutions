// CMyViewer.cpp : implementation file
//

#include "stdafx.h"
#include "FirstTask.h"
#include "CMyViewer.h"


// CMyViewer

IMPLEMENT_DYNAMIC(CMyViewer, CWnd)

CMyViewer::CMyViewer()
{

}

CMyViewer::~CMyViewer()
{
}


BEGIN_MESSAGE_MAP(CMyViewer, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyViewer message handlers

bool CMyViewer::loadNewImg()
{
	CMyImage *tmp = nullptr;
	if ((fifo == nullptr) || ((tmp = fifo->GetReady()) == nullptr)) {
		return false;
	}

	img = tmp;
	fifo->AddFree();

	return true;
}



void CMyViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	if (img == nullptr) {
		Sleep(0);
		ReleaseDC(&dc);
		return;
	}

	

	img->DrawImage(dc);

	
}
