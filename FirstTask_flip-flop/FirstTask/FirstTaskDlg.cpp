#include "stdafx.h"
#include "FirstTask.h"
#include "FirstTaskDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define SET_RANGE(color) m_Spin ## color.SetRange(0, 255); \
						 m_Slider ## color.SetRange(0, 255);

#define CASE_TICKER(clr, clr_name) case ct ## clr: \
										tmp.Format(L"%d", color. ## clr_name); \
										m_Edit ## clr.SetWindowTextW(tmp); \
										m_Slider ## clr.SetPos(color. ## clr_name); \
										m_Spin ## clr.SetPos(color. ## clr_name); \
										break;


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CFirstTaskDlg



CFirstTaskDlg::CFirstTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FIRSTTASK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFirstTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_R, m_EditR);
	DDX_Control(pDX, IDC_EDIT_G, m_EditG);
	DDX_Control(pDX, IDC_EDIT_B, m_EditB);
	DDX_Control(pDX, IDC_SPIN_R, m_SpinR);
	DDX_Control(pDX, IDC_SPIN_G, m_SpinG);
	DDX_Control(pDX, IDC_SPIN_B, m_SpinB);
	DDX_Control(pDX, IDC_SLIDER_R, m_SliderR);
	DDX_Control(pDX, IDC_SLIDER_G, m_SliderG);
	DDX_Control(pDX, IDC_SLIDER_B, m_SliderB);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_MyPaint);
}



BEGIN_MESSAGE_MAP(CFirstTaskDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_R, &CFirstTaskDlg::OnDeltaposSpinR)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_G, &CFirstTaskDlg::OnDeltaposSpinG)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_B, &CFirstTaskDlg::OnDeltaposSpinB)
	ON_EN_KILLFOCUS(IDC_EDIT_R, &CFirstTaskDlg::OnEnKillfocusEditR)
	ON_EN_KILLFOCUS(IDC_EDIT_G, &CFirstTaskDlg::OnEnKillfocusEditG)
	ON_EN_KILLFOCUS(IDC_EDIT_B, &CFirstTaskDlg::OnEnKillfocusEditB)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &CFirstTaskDlg::OnBnClickedButtonDraw)

	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CFirstTaskDlg

void CFirstTaskDlg::UpdateColorTickers(ColorType ct)
{
	CString tmp;
	switch (ct) {
	case ctR:
		genTimeout = color.rgbRed;
		break;
	case ctB:
		emptyTimeout = color.rgbBlue;
		break;
		CASE_TICKER(G, rgbGreen);
		//CASE_TICKER(B, rgbBlue);
	}
	OnBnClickedButtonDraw();
}

void CFirstTaskDlg::OnEditChange(CEdit &edit, ColorType ct)
{
	CString tmp;
	edit.GetWindowTextW(tmp);
	int i = _wtoi(tmp);
	if (i > 255 || i < 0) {
		i = (i > 255) ? 255 : 0;
		MessageBox(L"Out Of Range");
	}


	OnPosChange(i, ct);
	UpdateColorTickers(ct);

}

void CFirstTaskDlg::OnPosChange(int pos, ColorType ct)
{
	if (pos > 255 || pos < 0) {
		pos = (pos > 255) ? 255 : 0;
	}

	switch (ct) {
	case ctR:
		color.rgbRed = pos;
		break;
	case ctG:
		color.rgbGreen = pos;
		break;
	case ctB:
		color.rgbBlue = pos;
		break;
	}
	UpdateColorTickers(ct);
}

BOOL CFirstTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	SET_RANGE(R);
	SET_RANGE(G);
	SET_RANGE(B);

	/*
	for (auto &el : { IDC_RADIO_EXTHREAD, IDC_RADIO_SSLOW, IDC_CHECK_FF }) {
		CButton* pButton = (CButton*)GetDlgItem(el);
		pButton->SetCheck(true);
	}
	*/



	generator= std::thread(&CFirstTaskDlg::ThreadProc, this);
	//generator.detach();

	SetTimer(1, 100, NULL);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CFirstTaskDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CFirstTaskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CFirstTaskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFirstTaskDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (!(bShow && fifo == nullptr)) {
		return;
	}
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_PLACEHOLDER);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	//

	fifo = s_ptr<CFlipFlop<CMyImage>>(new CFlipFlop<CMyImage>(rect.right, rect.bottom));
	m_MyPaint.SetFiFo(fifo);

}



void CFirstTaskDlg::OnDeltaposSpinR(NMHDR *pNMHDR, LRESULT *pResult)
{
	{
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		*pResult = 0;
	}
	OnPosChange(m_SpinR.GetPos(), ctR);
}


void CFirstTaskDlg::OnDeltaposSpinG(NMHDR *pNMHDR, LRESULT *pResult)
{
	{
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		*pResult = 0;
	}
	OnPosChange(m_SpinG.GetPos(), ctG);
}


void CFirstTaskDlg::OnDeltaposSpinB(NMHDR *pNMHDR, LRESULT *pResult)
{
	{
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
		*pResult = 0;
	}
	OnPosChange(m_SpinB.GetPos(), ctB);
}



void CFirstTaskDlg::OnEnKillfocusEditR()
{
	OnEditChange(m_EditR, ctR);
}


void CFirstTaskDlg::OnEnKillfocusEditG()
{
	OnEditChange(m_EditG, ctG);
}


void CFirstTaskDlg::OnEnKillfocusEditB()
{
	OnEditChange(m_EditB, ctB);
}


void CFirstTaskDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	switch (nSBCode) {
	case TB_THUMBTRACK:
	case TB_THUMBPOSITION:
		if (pScrollBar->m_hWnd == m_SliderR.m_hWnd) {
			OnPosChange(nPos, ctR);
		}

		if (pScrollBar->m_hWnd == m_SliderG.m_hWnd) {
			OnPosChange(nPos, ctG);
		}

		if (pScrollBar->m_hWnd == m_SliderB.m_hWnd) {
			OnPosChange(nPos, ctB);
		}
		break;
	default:
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}

}


void CFirstTaskDlg::OnBnClickedButtonDraw()
{

}


void CFirstTaskDlg::OnTimer(UINT_PTR  nIDEvent)
{
	if (nIDEvent == 1)
	{
		Sleep(m_SpinG.GetPos() * 10);
		m_MyPaint.Invalidate();
		m_MyPaint.UpdateWindow();
	}
	CWnd::OnTimer(nIDEvent);
}


void CFirstTaskDlg::ThreadProc()
{
	while (!threadStop) {

		CMyImage *img = nullptr;

		if ( (fifo == nullptr) || ((img = fifo->GetFree()) == nullptr) ) {
			Sleep(emptyTimeout * 10);
			continue;
		}

		img->GenerateImage();
		fifo->AddReady();

		Sleep(genTimeout * 10);
	}
	threadStop = false;
}