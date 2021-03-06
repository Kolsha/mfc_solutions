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
	//image_mutex.lock();
	std::thread generator(&CMyViewer::threadProc, this);
	generator.detach();
}

CMyViewer::~CMyViewer()
{
	threadSuicide = true;
	image_mutex.lock();
	delete[] this->image;
}


BEGIN_MESSAGE_MAP(CMyViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// Обработчики сообщений CMyViewer




void CMyViewer::OnPaint()
{
	if (!InitImage()) {
		return;
	}
	CPaintDC dc(this);
	BITMAPINFO bif;

	{
		
		ZeroMemory(&bif, sizeof(BITMAPINFO));
		bif.bmiHeader.biSize = sizeof(bif);
		bif.bmiHeader.biHeight = -image_size.bottom;
		bif.bmiHeader.biWidth = image_size.right;
		bif.bmiHeader.biSizeImage = ((bif.bmiHeader.biWidth * 24 + 31) & ~31) / 8 * bif.bmiHeader.biHeight;
		bif.bmiHeader.biPlanes = 1;
		bif.bmiHeader.biBitCount = sizeof(RGBQUAD) * 8;
	}

	if (need_generate) {
		switch (threadMode) {
		case 0:
			GenImage();
			break;
		//default:
			///image_mutex.unlock();
		}
	}

	SetDIBitsToDevice(dc, 0, 0, image_size.right, image_size.bottom, 0, 0, 0, image_size.bottom, image, &bif, DIB_RGB_COLORS);

	need_generate = true;
}


BOOL CMyViewer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	return CWnd::OnEraseBkgnd(pDC);
}

bool CMyViewer::InitImage()
{
	if (this->image != nullptr) {
		return true;
	}
	try {
		GetWindowRect(&image_size);
		ScreenToClient(&image_size);
		this->image = new RGBQUAD[image_size.right * image_size.bottom];
	}
	catch (...) {
		this->image = nullptr;
	}

	return (this->image != nullptr);
}

void CMyViewer::GenImage()
{
	
	for (int y = 0; y < image_size.bottom; y++)
	{
		for (int x = 0; x < image_size.right; x++)
		{
			if (threadSuicide)
				return;
			if (image_size.right * y + x < image_size.right * image_size.bottom)
			{
				int pos = image_size.right * y + x;
				image[pos].rgbBlue = BYTE(color.rgbBlue + (x * y));
				image[pos].rgbGreen = BYTE(color.rgbGreen + y);
				image[pos].rgbRed = BYTE(color.rgbRed + (x + y));
				image[pos].rgbReserved = 0;
				
			}
			if (speedMode == 2) {
				Sleep(1);
			}
		}
		if (speedMode == 1) {
			Sleep(1);
		}
	}

	if (speedMode == 0) {
		Sleep(100);
	}

	
}

void CMyViewer::threadProc()
{
	
	RGBQUAD prevColor = color;
	for (;;) {
		
		if (threadMode == 0 || (
			prevColor.rgbBlue == color.rgbBlue &&
			prevColor.rgbRed == color.rgbRed && 
			prevColor.rgbGreen == color.rgbGreen
			)) {
			Sleep(1);
			continue;
		}

		image_mutex.lock();
		GenImage();
		need_generate = false;
		if (threadSuicide) {
			image_mutex.unlock();
			return;
		}
		Invalidate();
		UpdateWindow();
		image_mutex.unlock();

	}
}
