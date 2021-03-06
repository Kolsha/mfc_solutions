// CMyViewer.cpp: файл реализации
//

#include "stdafx.h"
#include "FirstTask.h"
#include "CMyViewer.h"
#include <thread>
#include <mutex>


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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// Обработчики сообщений CMyViewer




void CMyViewer::OnPaint()
{
	if (fifo == nullptr) {
		return;
	}

	CMyImage *img = fifo->GetReady();
	if (img == nullptr) {
		//OutputDebugStringW(L"Empty ready\n");
		Sleep(0);
		return;
	}

	CPaintDC dc(this);
	
	img->DrawImage(dc);
	fifo->AddFree();
	ReleaseDC(&dc);
}


BOOL CMyViewer::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


template<typename T>
T& fromVlt(volatile T& other) {
	return *(const_cast<T*>(&other));
}

inline bool operator==(const RGBQUAD& lhs, const RGBQUAD& rhs) {

	return (
		lhs.rgbRed == rhs.rgbRed	&&
		lhs.rgbBlue == rhs.rgbBlue	&&
		lhs.rgbGreen == rhs.rgbGreen
		);

}
