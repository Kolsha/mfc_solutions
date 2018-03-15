#include "stdafx.h"
#include "CMyImage.h"


CMyImage::CMyImage(size_t _width, size_t _height, RGBQUAD _startColor) :
	startColor(_startColor),
	width(_width), height(_height),
	img(new RGBQUAD[width * height])
{

}



CMyImage::CMyImage(const CMyImage & other) : 
	startColor(other.startColor),
	width(other.width), height(other.height),
	img(new RGBQUAD[width * height])
{
	std::memcpy(img, other.img, (sizeof(RGBQUAD) * width * height));
}

CMyImage::CMyImage(CMyImage && other)
{
	if (this != &other)
	{
		width = other.width;
		height = other.height;
		startColor = other.startColor;

		img = other.img;
		other.img = nullptr;
	}
}

CMyImage & CMyImage::operator=(const CMyImage & other)
{
	if (this != &other)
	{
		std::lock_guard<std::mutex> lock(img_mtx);
		delete[] img;

		width = other.width;
		height = other.height;
		startColor = other.startColor;

		img = new RGBQUAD[width * height];
		std::memcpy(img, other.img, (sizeof(RGBQUAD) * width * height));
	}
	return *this;
}

CMyImage & CMyImage::operator=(CMyImage && other)
{
	if (this != &other)
	{
		std::lock_guard<std::mutex> lock(img_mtx);
		delete[] img;

		width = other.width;
		height = other.height;
		startColor = other.startColor;

		img = other.img;
		other.img = nullptr;
	}
	return *this;
}


void CMyImage::DrawImage(CPaintDC & dc)
{
	std::lock_guard<std::mutex> lock(img_mtx);
	BITMAPINFO bif;

	{

		ZeroMemory(&bif, sizeof(BITMAPINFO));
		bif.bmiHeader.biSize = sizeof(bif);
		bif.bmiHeader.biHeight = -long(height);
		bif.bmiHeader.biWidth = long(width);
		bif.bmiHeader.biSizeImage = ((bif.bmiHeader.biWidth * 24 + 31) & ~31) / 8 * bif.bmiHeader.biHeight;
		bif.bmiHeader.biPlanes = 1;
		bif.bmiHeader.biBitCount = sizeof(RGBQUAD) * 8;
	}

	

	SetDIBitsToDevice(dc, 0, 0, long(width), long(height), 0, 0, 0, UINT(height), img, &bif, DIB_RGB_COLORS);
}


volatile BYTE shift = 0;

void CMyImage::GenerateImage()
{
	if (img == nullptr) {
		return;
	}
	std::lock_guard<std::mutex> lock(img_mtx);

	shift += 5;

	//startColor = { BYTE(rand() + shift) , BYTE(rand()) , BYTE(rand()) , BYTE(rand()) };
	startColor.rgbRed += 1;
	startColor.rgbBlue -= 1;

	startColor.rgbGreen = shift;

	if ((startColor.rgbRed % 3) == 0)
		startColor.rgbGreen++;

	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			if (width * y + x < width * height)
			{
				size_t pos = width * y + x;
				img[pos].rgbBlue = BYTE(startColor.rgbBlue + sqrt(x*x - y * y));
				img[pos].rgbGreen = BYTE(startColor.rgbGreen + y);
				img[pos].rgbRed = BYTE(startColor.rgbRed + sqrt(x*x + y * y));
				img[pos].rgbReserved = 0;

			}

		}

	}
}

CMyImage::~CMyImage()
{
	std::lock_guard<std::mutex> lock(img_mtx);
	delete[] img;
}
