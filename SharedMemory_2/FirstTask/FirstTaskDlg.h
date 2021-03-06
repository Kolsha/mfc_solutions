#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "CMyViewer.h"
#include "CFiFo.hpp"
#include "CMyImage.h"

#include "CMyProgressBar.h"

/*

myviewer дерни из фифошки
edit

*/

class CFirstTaskDlg : public CDialog
{
public:
	CFirstTaskDlg(CWnd* pParent = NULL);

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIRSTTASK_DIALOG };
#endif

protected:

	static const UINT_PTR imgPaintTimerEvent = 777;
	static const UINT_PTR progressUpdateTimerEvent = 666;

	virtual void DoDataExchange(CDataExchange* pDX);	

	std::thread generator;
	s_ptr<CMyImage> generator_img = nullptr;

	s_ptr<CFiFo<RGBQUAD>> fifo = nullptr;

	size_t genEmpty = 0, readerEmpty = 0;

	size_t genTimeOut = 0, readTimeOut = 10;

	size_t fifo_size = 1700;

	volatile bool threadStop = false;

	void ThreadProc();
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	CMyProgressBar m_MyProgressBar;
	CEdit m_EditEmpty1;
	CEdit m_EditEmpty2;

	afx_msg void OnTimer(UINT_PTR nIDEvent);


	virtual ~CFirstTaskDlg() {
		threadStop = true;
		if (generator.joinable()) {
			generator.join();
			while (threadStop);
		}

		
			
	}

	
	CMyViewer m_MyPaint;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_SliderGenTimeOut;
	CSliderCtrl m_SliderReadTimeOut;
	afx_msg void OnBnClickedButton1();
};
