
// MySocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"
#include "MySocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMySocketDlg dialog



CMySocketDlg::CMySocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYSOCKET_DIALOG, pParent)
	, m_cstrLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR_IP, m_edtIP);
	DDX_Control(pDX, IDC_EDTTALK, m_edtTalk);
	DDX_Control(pDX, IDC_HOSTIP, m_csHostIP);
	DDX_Text(pDX, IDC_EDTLOG, m_cstrLog);
	DDV_MaxChars(pDX, m_cstrLog, 4096);
}

BEGIN_MESSAGE_MAP(CMySocketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CMySocketDlg::OnBnClickedConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEND, &CMySocketDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_HOST, &CMySocketDlg::OnBnClickedHost)
END_MESSAGE_MAP()


// CMySocketDlg message handlers

BOOL CMySocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CString str("HostIP:");
	m_csHostIP.SetWindowTextW(str+pApp->m_cstrHostIP);		//Display local IP
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMySocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMySocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMySocketDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return true;
	case VK_RETURN:
		if (m_edtTalk.GetWindowTextLengthW() > 0)
			OnBnClickedSend();
		return true;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMySocketDlg::OnBnClickedConnect()
{
	m_sktSock.ShutDown(2);
	m_sktSock.m_hSocket = INVALID_SOCKET;
	m_sktSock.SetStatus(FALSE);
	m_nPort = 1088;
	if (m_edtIP.GetWindowTextW(m_szServerAddr, sizeof(m_szServerAddr)))		//Get target IP
	{
		SetTimer(0,1000,NULL);		//Set timer to connect
		m_nTryCount = 0;
	}
}



void CMySocketDlg::OnBnClickedSend()
{
	if (m_sktSock.IsConnected())
	{
		Msg temp;
		temp.ctTime = CTime::GetCurrentTime();
		temp.nType = TP_TALK;
		m_edtTalk.GetWindowTextW(temp.szValue, sizeof(temp.szValue));
		m_sktSock.m_nLength = sizeof(temp);
		m_smMsg.Assign(temp);
		m_sktSock.AsyncSelect(FD_WRITE);
		m_edtTalk.SetWindowTextW(_T(""));
	}
}


void CMySocketDlg::OnBnClickedHost()
{
	m_sktSock.ShutDown(2);
	m_sktSock.m_hSocket = INVALID_SOCKET;
	m_sktSock.SetStatus(FALSE);
	m_nPort = 1088;
	if (m_sktSock.m_hSocket == INVALID_SOCKET)		//re-create valid socket
	{
		BOOL bFlag = m_sktSock.Create(m_nPort, SOCK_STREAM, FD_ACCEPT);
		if (!bFlag)
		{
			AfxMessageBox(_T("Socket Error!"));
			m_sktSock.Close();
			return;
		}
	}
}

void CMySocketDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_sktSock.m_hSocket == INVALID_SOCKET)		//re-create valid socket
	{
		BOOL bFlag = m_sktSock.Create(m_nPort, SOCK_STREAM, FD_CONNECT);
		if (!bFlag)
		{
			AfxMessageBox(_T("Socket Error!"));
			m_sktSock.Close();
			return;
		}
	}
	m_sktSock.Connect(m_szServerAddr, m_nPort);
	m_nTryCount++;
	if (m_nTryCount > 10 || m_sktSock.IsConnected())
	{
		KillTimer(0);
		if (m_nTryCount > 10)
			AfxMessageBox(_T("Out of time!"));
		return;
	}
	CDialogEx::OnTimer(nIDEvent);
}
