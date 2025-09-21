
// MFCThreadDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCThread.h"
#include "MFCThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 自定义消息
#define WM_THREAD_UPDATE (WM_USER + 100)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCThreadDlg 对话框



CMFCThreadDlg::CMFCThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTHREAD_DIALOG, pParent)
	, m_butAntry(FALSE)
	, m_wThreadWorker(nullptr)
	,m_bPause(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCThreadDlg::~CMFCThreadDlg()
{
	if (m_wThreadWorker && m_wThreadWorker->IsRunning())
	{
		m_wThreadWorker->StopThread();
		//SetWindowText(L"线程已停止");
	}
	if (m_wThreadWorker)
	{
		delete m_wThreadWorker;
		m_wThreadWorker = nullptr;
	}
}

void CMFCThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(CMFCThreadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCThreadDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_THREAD_UPDATE, &CMFCThreadDlg::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCThreadDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCThreadDlg 消息处理程序

BOOL CMFCThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 初始化进度条
	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);
	m_wThreadWorker = new Worker(this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout; 
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCThreadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCThreadDlg::OnBnClickedButton1()
{
	if (!m_wThreadWorker) return;
	// TODO: 在此添加控件通知处理程序代码
	m_butAntry = !m_butAntry;
	if (m_butAntry)
	{
		if (!m_wThreadWorker->IsRunning())
		{
			m_wThreadWorker->StopThread();
		}
		m_progress.SetPos(0);
		if (m_wThreadWorker->StartThread())
		{
			SetWindowText(L"线程已启动 - 进度: 0%");
		}
		else
		{
			AfxMessageBox(L"启动线程失败！");
		}
	}
	else
	{
		if (m_wThreadWorker && m_wThreadWorker->IsRunning())
		{
			m_wThreadWorker->StopThread();
			SetWindowText(L"线程已停止");
		}
	}

}


LRESULT CMFCThreadDlg::OnNMCustomdrawProgress1(WPARAM wParam, LPARAM lParam)
{
	int nProgress = (int)wParam;
	BOOL bFinished = (BOOL)lParam;

	// 更新进度条
	m_progress.SetPos(nProgress);

	// 更新对话框标题
	CString strTitle;
	if (bFinished)
	{
		strTitle.Format(L"完成 - 进度: %d%%", nProgress);
	}
	else
	{
		strTitle.Format(L"运行中 - 进度: %d%%", nProgress);
	}
	SetWindowText(strTitle);

	return 0;

}


void CMFCThreadDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_wThreadWorker->IsRunning()) return;
	m_bPause = !m_bPause;
	if (m_bPause)
	{
		m_wThreadWorker->PauseThread();
	}
	else
	{
		m_wThreadWorker->ContinueThread();
	}
}
