#pragma once

#include <mutex>

#include "CMyImage.h"
#include "CFiFo.h"
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
	inline void SetFiFo(s_ptr<IReadFiFo<CMyImage>> _fifo) {
		this->fifo = _fifo;
	}
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:

	s_ptr<IReadFiFo<CMyImage>> fifo = nullptr;

};


