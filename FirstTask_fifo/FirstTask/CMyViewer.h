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
	s_ptr<IReadFiFo<CMyImage>> fifo = nullptr;
	CMyImage *img = nullptr;
protected:
	DECLARE_MESSAGE_MAP()
public:
	inline void SetFiFo(s_ptr<IReadFiFo<CMyImage>> _fifo) {
		this->fifo = _fifo;
	}
	bool loadNewImg();

	afx_msg void OnPaint();
};


