#pragma once


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
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


