#pragma once


// CMyProgressBar

class CMyProgressBar : public CWnd
{
	DECLARE_DYNAMIC(CMyProgressBar)
private:
	size_t max_pos = 100;
	size_t left_pos = 0;

	size_t right_pos = 30;
public:
	CMyProgressBar();

	void setMaxPos(size_t _pos) {
		max_pos = _pos;
	}
	void setLeftPos(size_t _pos) {
		left_pos = _pos;

	}

	void setRightPos(size_t _pos) {
		right_pos = _pos;

	}

	void Update() {
		Invalidate();
		UpdateWindow();
	}
	virtual ~CMyProgressBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


