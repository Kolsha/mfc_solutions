#pragma once
#include <mutex>

class CMyImage
{
private:
	RGBQUAD startColor = { 0, 0, 0, 0 };
	size_t width, height;
	RGBQUAD * img = nullptr;
	
	std::mutex img_mtx; // because object can be deleted while draw/generate img
public:
	CMyImage(size_t _width = 512, size_t _height = 512, RGBQUAD _startColor = { 0, 0, 0, 0 });

	
	CMyImage(const CMyImage& other); // copy constructor
				 
	CMyImage(CMyImage&& other); // Move constructor.  


	CMyImage& operator=(const CMyImage& other);  // Copy assignment operator.  

	CMyImage& operator=(CMyImage&& other); // Move assignment operator. 
	

	inline size_t GetWidth() { return width; }
	inline size_t GetHeight() { return height; }

	inline void SetColor(RGBQUAD _startColor) {
		startColor = _startColor;
	}


	

	void DrawImage(CPaintDC &dc);
	void GenerateImage();
	virtual ~CMyImage();

	//copy and move constructor need
};

