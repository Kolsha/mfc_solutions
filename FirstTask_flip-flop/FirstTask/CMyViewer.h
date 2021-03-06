#pragma once

#include <mutex>

#include "CMyImage.h"
#include "CFiFo.h"


class CMyViewer : public CWnd
{
	DECLARE_DYNAMIC(CMyViewer)



protected:
	DECLARE_MESSAGE_MAP()
public:
	CMyViewer();
	
	inline void SetFiFo(s_ptr<IReadFiFo<CMyImage>> _fifo) {
		this->fifo = _fifo;
	}

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual ~CMyViewer();
private:

	s_ptr<IReadFiFo<CMyImage>> fifo = nullptr;

};


