#pragma once


#include "CMyImage.h"
#include "CFiFo.h"

class CMyViewer : public CWnd
{
	DECLARE_DYNAMIC(CMyViewer)

public:
	CMyViewer();
	virtual ~CMyViewer();
private:
	s_ptr<IReadFiFo<RGBQUAD>> fifo = nullptr;

	s_ptr<CMyImage> img = nullptr;
protected:
	DECLARE_MESSAGE_MAP()
public:
	inline void SetFiFo(s_ptr<IReadFiFo<RGBQUAD>> _fifo) {
		this->fifo = _fifo;
	}

	inline void SetImage(s_ptr<CMyImage> _img) {
		this->img = _img;
	}
	bool loadNewImg();

	afx_msg void OnPaint();
};


