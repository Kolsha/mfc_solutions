#pragma once

#include <mutex>

// CMyViewer

class CMyViewer : public CWnd
{
	DECLARE_DYNAMIC(CMyViewer)

public:
	CMyViewer();
	virtual ~CMyViewer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	RGBQUAD color;
	volatile int threadMode = 0;
	int speedMode = 0;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	RGBQUAD *image = nullptr;
	RECT image_size;
	std::mutex image_mutex;

	volatile bool need_generate = true;
	bool InitImage();
	void GenImage();

	void threadProc();

	volatile bool threadSuicide = false;

	

};


