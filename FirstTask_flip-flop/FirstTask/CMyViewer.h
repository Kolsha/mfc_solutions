#pragma once

#include <mutex>

// CMyViewer

static const size_t bufferCount = 2;

class CMyViewer : public CWnd
{
	DECLARE_DYNAMIC(CMyViewer)

public:
	CMyViewer();
	virtual ~CMyViewer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void setColor(RGBQUAD newColor);

	volatile int threadMode = 0;
	int speedMode = 0;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:

	volatile RGBQUAD color;

	RGBQUAD *images = nullptr;
	RECT image_size;

	volatile size_t bufferIdx = 0;
	
	std::mutex image_mutex, idx_mutex;

	bool InitImage();
	void GenImage(RGBQUAD startColor);

	void threadProc();

	volatile bool threadSuicide = false;




};


