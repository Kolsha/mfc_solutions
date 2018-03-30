#include "stdafx.h"
#include "CMyImage.h"


void CMyImage::genBitmapInfo()
{
		ZeroMemory(&bif, sizeof(BITMAPINFO));
		bif.bmiHeader.biSize = sizeof(bif);
		bif.bmiHeader.biHeight = -long(height);
		bif.bmiHeader.biWidth = long(width);
		bif.bmiHeader.biSizeImage = ((bif.bmiHeader.biWidth * 24 + 31) & ~31) / 8 * bif.bmiHeader.biHeight;
		bif.bmiHeader.biPlanes = 1;
		bif.bmiHeader.biBitCount = sizeof(RGBQUAD) * 8;
}

CMyImage::CMyImage(RGBQUAD *memory, size_t _width, size_t _height)
{
	img = memory;
	need_delete = false;
}

CMyImage::CMyImage(size_t _width, size_t _height, RGBQUAD _startColor) :
	startColor(_startColor),
	width(_width), 
	height(_height),
	img(new RGBQUAD[width * height])
{
	genBitmapInfo();
}



CMyImage::CMyImage(const CMyImage & other) : 
	startColor(other.startColor),
	width(other.width), height(other.height),
	img(new RGBQUAD[width * height])
{
	std::memcpy(img, other.img, (sizeof(RGBQUAD) * width * height));
	genBitmapInfo();
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
	genBitmapInfo();
}

CMyImage & CMyImage::operator=(const CMyImage & other)
{
	if (this != &other)
	{
		std::lock_guard<std::mutex> lock(img_mtx);
		width = other.width;
		height = other.height;
		startColor = other.startColor;
		if (need_delete) {
			delete[] img;
			img = new RGBQUAD[width * height];
		}
		std::memcpy(img, other.img, (sizeof(RGBQUAD) * width * height));
	}
	genBitmapInfo();
	return *this;
}

CMyImage & CMyImage::operator=(CMyImage && other)
{
	if (this != &other)
	{
		std::lock_guard<std::mutex> lock(img_mtx);
		if(need_delete)
			delete[] img;

		width = other.width;
		height = other.height;
		startColor = other.startColor;

		img = other.img;
		other.img = nullptr;
	}
	genBitmapInfo();
	return *this;
}


void CMyImage::SetMem(RGBQUAD * memory, bool _need_delete, size_t _width, size_t _height)
{
	if (memory == nullptr) {
		return;
	}
	std::lock_guard<std::mutex> lock(img_mtx);
	if (img != nullptr && need_delete) {
		delete[] img;
	}

	img = memory;
	need_delete = _need_delete;

	if(_width > 0)
		width = _width;
	if(_height > 0)
		height = _height;
}

void CMyImage::DrawImage(CPaintDC & dc)
{
	std::lock_guard<std::mutex> lock(img_mtx);

	SetDIBitsToDevice(dc, 0, 0, long(width), long(height), 0, 0, 0, UINT(height), img, &bif, DIB_RGB_COLORS);
}


volatile BYTE shift = 0;

void CMyImage::GenerateImage()
{
	if (img == nullptr) {
		return;
	}
	std::lock_guard<std::mutex> lock(img_mtx);

	shift += 1;

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
				img[pos].rgbBlue = 0;// BYTE(startColor.rgbBlue + sqrt(x*x - y * y));
				img[pos].rgbGreen = BYTE(startColor.rgbGreen + x);
				img[pos].rgbRed = 0;// BYTE(startColor.rgbRed + sqrt(x*x + y * y));
				img[pos].rgbReserved = 0;

			}

		}

	}
}

CMyImage::~CMyImage()
{
	if (need_delete) {
		std::lock_guard<std::mutex> lock(img_mtx);
		delete[] img;
	}
}
