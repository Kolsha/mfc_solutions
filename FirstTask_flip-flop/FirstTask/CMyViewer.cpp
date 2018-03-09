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
	std::thread generator(&CMyViewer::threadProc, this);
	generator.detach();
}

CMyViewer::~CMyViewer()
{
	threadSuicide = true;
	image_mutex.lock();
	delete[] this->images;
}


BEGIN_MESSAGE_MAP(CMyViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// Обработчики сообщений CMyViewer




void CMyViewer::setColor(RGBQUAD newColor)
{
	color.rgbRed = newColor.rgbRed;
	color.rgbBlue = newColor.rgbBlue;
	color.rgbGreen = newColor.rgbGreen;
}

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

	if (threadMode == 0) {
		GenImage(*(const_cast<RGBQUAD*>(&color)));
	}

	RGBQUAD *image = nullptr;
	{
		std::lock_guard<std::mutex> lock(idx_mutex);
		image = &images[bufferIdx * image_size.right * image_size.bottom];
	}

	SetDIBitsToDevice(dc, 0, 0, image_size.right, image_size.bottom, 0, 0, 0, image_size.bottom, image, &bif, DIB_RGB_COLORS);


	ReleaseDC(&dc);


}


BOOL CMyViewer::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

bool CMyViewer::InitImage()
{
	if (this->images != nullptr) {
		return true;
	}
	try {
		GetWindowRect(&image_size);
		ScreenToClient(&image_size);
		this->images = new RGBQUAD[bufferCount * image_size.right * image_size.bottom];
	}
	catch (...) {
		this->images = nullptr;
	}

	return (this->images != nullptr);
}

void CMyViewer::GenImage(RGBQUAD startColor)
{
	std::lock_guard<std::mutex> lock(image_mutex);

	RGBQUAD *image = nullptr;
	{
		std::lock_guard<std::mutex> lock(idx_mutex);
		image = &images[bufferIdx * image_size.right * image_size.bottom];
		//ZeroMemory(image, sizeof(RGBQUAD) * image_size.right * image_size.bottom);

		bufferIdx = (++bufferIdx) % bufferCount;
	}


	for (int x = 0; x < image_size.right; x++)
	{
		for (int y = 0; y < image_size.bottom; y++)
		{
			if (threadSuicide)
				return;
			if (image_size.right * y + x < image_size.right * image_size.bottom)
			{
				int pos = image_size.right * y + x;
				image[pos].rgbBlue = BYTE(startColor.rgbBlue + (x * y));
				image[pos].rgbGreen = BYTE(startColor.rgbGreen + y);
				image[pos].rgbRed = BYTE(startColor.rgbRed + (x + y));
				image[pos].rgbReserved = 0;

			}
			if (speedMode == 2) {
				Sleep(0);
			}


		}
		if (speedMode == 1) {
			Sleep(1);
		}
	}

	if (speedMode == 0) {
		Sleep(100);
	}

	OutputDebugString(_T("End gen."));
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

void CMyViewer::threadProc()
{

	volatile RGBQUAD prevColor;
	prevColor.rgbRed = color.rgbRed;
	prevColor.rgbBlue = color.rgbBlue;
	prevColor.rgbGreen = color.rgbGreen;
	for (;;) {

		if (threadMode == 0 ||
			fromVlt<RGBQUAD>(prevColor) == fromVlt<RGBQUAD>(color)) {
			Sleep(0);
			continue;
		}
		fromVlt<RGBQUAD>(prevColor) = fromVlt<RGBQUAD>(color);

		GenImage(fromVlt<RGBQUAD>(prevColor));

		if (threadSuicide) {
			return;
		}

		Invalidate();
		UpdateWindow();

	}
}
