#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "CMyViewer.h"
#include "CFiFo.h"
#include "CMyImage.h"


class CFirstTaskDlg : public CDialog
{
public:
	CFirstTaskDlg(CWnd* pParent = NULL);

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIRSTTASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


	s_ptr<CFiFo<CMyImage>> fifo = nullptr;

	volatile bool threadStop = false;

	void ThreadProc();
	HICON m_hIcon;

	RGBQUAD color = { 0, 0, 0, 0 };

	enum ColorType { ctR, ctG, ctB };

	void UpdateColorTickers(ColorType ct);

	void OnEditChange(CEdit &edit, ColorType ct);
	void OnPosChange(int pos, ColorType ct);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyViewer m_MyPaint;
	CEdit m_EditR;
	CEdit m_EditG;
	CEdit m_EditB;
	CSpinButtonCtrl m_SpinR;
	CSpinButtonCtrl m_SpinG;
	CSpinButtonCtrl m_SpinB;
	CSliderCtrl m_SliderR;
	CSliderCtrl m_SliderG;
	CSliderCtrl m_SliderB;
	afx_msg void OnDeltaposSpinR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditR();
	afx_msg void OnEnKillfocusEditG();
	afx_msg void OnEnKillfocusEditB();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedRadioMthread();
	afx_msg void OnBnClickedRadioExthread();
	afx_msg void OnBnClickedRadioSnormal();
	afx_msg void OnBnClickedRadioSslow();
	afx_msg void OnBnClickedRadioSvslow();
	afx_msg void OnBnClickedCheckFf();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_CheckFF;

	virtual ~CFirstTaskDlg() {
		threadStop = true;
	}
};
