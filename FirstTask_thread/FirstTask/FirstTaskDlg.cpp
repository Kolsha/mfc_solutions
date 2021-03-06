
// FirstTaskDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "FirstTask.h"
#include "FirstTaskDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	ON_BN_CLICKED(IDC_RADIO_MTHREAD, &CFirstTaskDlg::OnBnClickedRadioMthread)
	ON_BN_CLICKED(IDC_RADIO_EXTHREAD, &CFirstTaskDlg::OnBnClickedRadioExthread)
	ON_BN_CLICKED(IDC_RADIO_SNORMAL, &CFirstTaskDlg::OnBnClickedRadioSnormal)
	ON_BN_CLICKED(IDC_RADIO_SSLOW, &CFirstTaskDlg::OnBnClickedRadioSslow)
	ON_BN_CLICKED(IDC_RADIO_SVSLOW, &CFirstTaskDlg::OnBnClickedRadioSvslow)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_R, &CFirstTaskDlg::OnTRBNThumbPosChangingSliderR)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_B, &CFirstTaskDlg::OnTRBNThumbPosChangingSliderB)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// Обработчики сообщений CFirstTaskDlg

void CFirstTaskDlg::UpdateColorTickers(ColorType ct)
{
	CString tmp;
	switch (ct) {
	case ctR:
		tmp.Format(L"%d", cR);
		m_EditR.SetWindowTextW(tmp);
		m_SliderR.SetPos(cR);
		m_SpinR.SetPos(cR);
		break;
	case ctG:
		tmp.Format(L"%d", cG);
		m_EditG.SetWindowTextW(tmp);
		m_SliderG.SetPos(cG);
		m_SpinG.SetPos(cG);
		break;
	case ctB:
		tmp.Format(L"%d", cB);
		m_EditB.SetWindowTextW(tmp);
		m_SliderB.SetPos(cB);
		m_SpinB.SetPos(cB);
		break;
	}

	m_MyPaint.color = { BYTE(cB), BYTE(cG), BYTE(cR), 0 };
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
	if (pos > 255 || pos < 0)
		pos = 125;
	switch (ct) {
	case ctR:
		cR = pos;
		break;
	case ctG:
		cG = pos;
		break;
	case ctB:
		cB = pos;
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

	m_SpinR.SetRange(0, 255);
	m_SpinG.SetRange(0, 255);
	m_SpinB.SetRange(0, 255);

	m_SliderR.SetRange(0, 255);
	m_SliderG.SetRange(0, 255);
	m_SliderB.SetRange(0, 255);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MTHREAD);
	pButton->SetCheck(true);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_SNORMAL);
	pButton->SetCheck(true);



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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

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



void CFirstTaskDlg::OnDeltaposSpinR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
	//UpdateColorTickers(ctR);
	OnPosChange(m_SpinR.GetPos(), ctR);
	
}


void CFirstTaskDlg::OnDeltaposSpinG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
	OnPosChange(m_SpinG.GetPos(), ctG);
}


void CFirstTaskDlg::OnDeltaposSpinB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;

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
	m_MyPaint.Invalidate();
	m_MyPaint.UpdateWindow();
}



void CFirstTaskDlg::OnBnClickedRadioMthread()
{
	m_MyPaint.threadMode = 0;
}


void CFirstTaskDlg::OnBnClickedRadioExthread()
{
	m_MyPaint.threadMode = 1;
}


void CFirstTaskDlg::OnBnClickedRadioSnormal()
{
	m_MyPaint.speedMode = 0;
}


void CFirstTaskDlg::OnBnClickedRadioSslow()
{
	m_MyPaint.speedMode = 1;
}


void CFirstTaskDlg::OnBnClickedRadioSvslow()
{
	m_MyPaint.speedMode = 2;
}



void CFirstTaskDlg::OnTRBNThumbPosChangingSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Для этого средства требуется Windows Vista или более поздняя версия.
	// Символ _WIN32_WINNT должен быть >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
	OnPosChange(m_SliderR.GetPos(), ctR);
}


void CFirstTaskDlg::OnTRBNThumbPosChangingSliderB(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Для этого средства требуется Windows Vista или более поздняя версия.
	// Символ _WIN32_WINNT должен быть >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;

}


void CFirstTaskDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	switch (nSBCode) {
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
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}
