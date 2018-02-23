// CMyViewer.cpp: файл реализации
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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// Обработчики сообщений CMyViewer




void CMyViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: добавьте свой код обработчика сообщений
					   // Не вызывать CWnd::OnPaint() для сообщений рисования

	BITMAPINFO bif;
	ZeroMemory(&bif, sizeof(BITMAPINFO));
	RECT r;
	GetWindowRect(&r);
	ScreenToClient(&r);

	bif.bmiHeader.biSize = sizeof(bif);

	RGBQUAD *im = new RGBQUAD[r.right*r.bottom];

	//RGBQUAD tmpColor = { 0, 0 , 0, 0 };

	/*for (int i = 0; i < r.right * r.bottom; i++)
	{
		
		im[i].rgbBlue = color.rgbBlue + tmpColor.rgbBlue++;
		im[i].rgbRed = color.rgbRed + tmpColor.rgbRed++;
		im[i].rgbGreen = color.rgbGreen + tmpColor.rgbGreen++;
		
		
		im[i].rgbBlue = i;
		im[i].rgbGreen = i;
		im[i].rgbRed = i;
		
	} */


	for (int y = 0; y < r.bottom; y++)
		for (int x = 0; x < r.right; x++)
		{
			if (r.right * y + x < r.right*r.bottom)
			{
				int pos = r.right * y + x;
				im[pos].rgbBlue = BYTE(color.rgbBlue + (x + y));
				im[pos].rgbGreen = BYTE(color.rgbGreen + y);
				im[pos].rgbRed = BYTE(color.rgbRed + (x - y));
				im[pos].rgbReserved = 255;
				/*
				im[r.right * y + x].rgbBlue = y;
				im[r.right * y + x].rgbRed = x;
				im[r.right * y + x].rgbGreen = 0;

				im[r.right * y + x].rgbBlue = color.rgbBlue + tmpColor.rgbBlue++;
				im[r.right * y + x].rgbRed = color.rgbRed + tmpColor.rgbRed++;
				im[r.right * y + x].rgbGreen = color.rgbGreen + tmpColor.rgbGreen++;

				*/
			}
		}


	bif.bmiHeader.biHeight = -r.bottom;
	bif.bmiHeader.biWidth = r.right;
	bif.bmiHeader.biSizeImage = ((bif.bmiHeader.biWidth * 24 + 31) & ~31) / 8 * bif.bmiHeader.biHeight;
	bif.bmiHeader.biPlanes = 1;
	bif.bmiHeader.biBitCount = sizeof(RGBQUAD) * 8;

	SetDIBitsToDevice(dc, 0, 0, r.right, r.bottom, 0, 0, 0, r.bottom, im, &bif, DIB_RGB_COLORS);

	delete[] im;
}


BOOL CMyViewer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	return CWnd::OnEraseBkgnd(pDC);
}
