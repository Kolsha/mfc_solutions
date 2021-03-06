#include "stdafx.h"
#include "FirstTask.h"
#include "FirstTaskDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	DDX_Control(pDX, IDC_MYPROGRESS, m_MyProgressBar);
	DDX_Control(pDX, IDC_EDIT_EMPTY1, m_EditEmpty1);
	DDX_Control(pDX, IDC_EDIT_EMPTY2, m_EditEmpty2);

	DDX_Control(pDX, IDC_MYIMAGE, m_MyPaint);
	DDX_Control(pDX, IDC_SLIDER_GEN_TIMEOUT2, m_SliderGenTimeOut);
	DDX_Control(pDX, IDC_SLIDER_GEN_TIMEOUT, m_SliderReadTimeOut);
}

BEGIN_MESSAGE_MAP(CFirstTaskDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CFirstTaskDlg::OnBnClickedButton1)
END_MESSAGE_MAP()





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






	SetTimer(imgPaintTimerEvent, readTimeOut, NULL);

	SetTimer(progressUpdateTimerEvent, 300, NULL);

	m_SliderGenTimeOut.SetRange(0, 2500);
	m_SliderReadTimeOut.SetRange(0, 2500);

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
	CWnd *pWnd = GetDlgItem(IDC_MYIMAGE);
	pWnd->GetWindowRect(&rect);
	pWnd->ScreenToClient(&rect);


	m_MyPaint.SetImage(s_ptr<CMyImage>(new CMyImage(rect.right, rect.bottom)));

	generator_img = s_ptr<CMyImage>(new CMyImage(rect.right, rect.bottom));
	generator_img->GenerateImage();


	fifo = s_ptr<CFiFo<RGBQUAD>>(new CFiFo<RGBQUAD>((rect.right * rect.bottom), fifo_size, std::string("my_fifo"), true));

	if (!fifo->Init()) {
		MessageBox(L"Fifo init failed!");
	}

	m_MyPaint.SetFiFo(fifo);

}






void CFirstTaskDlg::OnTimer(UINT_PTR  nIDEvent)
{
	if (nIDEvent == imgPaintTimerEvent)
	{
		if (!m_MyPaint.loadNewImg()) {
			readerEmpty++;
		}
		else {
			m_MyPaint.Invalidate();
			m_MyPaint.UpdateWindow();
		}


		//MessageBox(L"DD");

	}

	if (nIDEvent == progressUpdateTimerEvent) {

		CString str;


		size_t maxSize = fifo->getSize();
		size_t readySize = fifo->getReadySize();

		size_t freeSize = fifo->getFreeSize();




		str.Format(L"Delta: %d \n"
			"Max size: %d \n"
			"Ready: %d \n"
			"Free: %d \n"
			, 0,
			maxSize,
			readySize,
			freeSize);
		OutputDebugString(str);


		m_MyProgressBar.setMaxPos(maxSize);
		m_MyProgressBar.setLeftPos(readySize);
		m_MyProgressBar.setRightPos(freeSize);
		m_MyProgressBar.Update();


		str.Format(L"%d", genEmpty);
		m_EditEmpty1.SetWindowTextW(str);

		str.Format(L"%d", readerEmpty);
		m_EditEmpty2.SetWindowTextW(str);
	}
	CWnd::OnTimer(nIDEvent);
}


void CFirstTaskDlg::ThreadProc()
{
	while (!threadStop) {



		if ((fifo == nullptr) || (generator_img == nullptr)) {
			Sleep(100);
			continue;
		}

		RGBQUAD *mem = fifo->GetFree();
		if (mem == nullptr) {
			Sleep(10);
			genEmpty++;
			continue;
		}

		generator_img->SetMem(mem);
		generator_img->GenerateImage();

		fifo->AddReady();
		Sleep(genTimeOut);
	}
	threadStop = false;
}





void CFirstTaskDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	switch (nSBCode) {
	case TB_THUMBTRACK:
	case TB_THUMBPOSITION:

		break;
	default:
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}

}


void CFirstTaskDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString tmp;

	switch (nSBCode) {
	case TB_THUMBTRACK:
	case TB_THUMBPOSITION:


		if (pScrollBar->m_hWnd == m_SliderGenTimeOut.m_hWnd) {
			genTimeOut = nPos;

			tmp.Format(L"%d", genTimeOut);
			OutputDebugString(tmp);

			OutputDebugString(L" : genTimeOut \n");
		}
		if (pScrollBar->m_hWnd == m_SliderReadTimeOut.m_hWnd) {
			readTimeOut = nPos;
			SetTimer(imgPaintTimerEvent, readTimeOut, NULL);

			tmp.Format(L"%d", readTimeOut);
			OutputDebugString(tmp);
			OutputDebugString(L" : readTimeOut \n");
		}
		break;
	default:
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}


void CFirstTaskDlg::OnBnClickedButton1()
{
	if (generator.joinable()) {
		threadStop = true;
		generator.join();
	}
	else {
		generator = std::thread(&CFirstTaskDlg::ThreadProc, this);
	}
}
