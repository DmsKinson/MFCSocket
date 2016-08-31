
// MySocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"
#include "MySocketDlg.h"
#include "afxdialogex.h"
#include "CSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMySocketDlg dialog

CMySocketDlg::CMySocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYSOCKET_DIALOG, pParent)
	, m_cstrLog(_T(""))
	, m_cstrContent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sktCSock = NULL;
	m_sktSSock = NULL;
}

BOOL CMySocketDlg::IsServer()
{
	return m_bIsServer;
}

void CMySocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR_IP, m_edtIP);
	DDX_Control(pDX, IDC_EDTTALK, m_edtTalk);
	DDX_Control(pDX, IDC_HOSTIP, m_csHostIP);
	DDX_Text(pDX, IDC_EDTLOG, m_cstrLog);
	DDX_Text(pDX, IDC_EDTCONTENT, m_cstrContent);
	DDX_Control(pDX, IDC_ABORT, m_btnAbort);
	DDX_Control(pDX, IDC_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_HOST, m_btnHost);
	DDX_Control(pDX, IDC_SEND, m_btnSend);
}

BEGIN_MESSAGE_MAP(CMySocketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CMySocketDlg::OnBnClickedConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEND, &CMySocketDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_HOST, &CMySocketDlg::OnBnClickedHost)
	ON_MESSAGE(WM_USER_RECVMSG, &CMySocketDlg::OnRecvMsg)
	ON_BN_CLICKED(IDC_ABORT, &CMySocketDlg::OnBnClickedAbort)
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
	handle = GetSafeHwnd();
	AddLog(_T("Domitory Game Platform v1.0"),NOW_TIME);
	CString str("HostIP:");
	m_csHostIP.SetWindowTextW(str+pApp->m_cstrHostIP);		//Display local IP
	m_nPort = 1088;
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


//收到消息的处理函数
LRESULT CMySocketDlg::OnRecvMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_smMsg.IsEmpty())
	{
		switch (m_smMsg.GetType())
		{
		case TP_LOG:
			AddLog(m_smMsg.GetCString(),m_smMsg.GetTime());
			break;
		case TP_TALK:
			AddContent(m_smMsg.GetCString(),_T("Teddy"),m_smMsg.GetTime());
			break;
		case TP_WUZIQI:
			break;
		case TP_PAODEKUAI:
			break;
		case TP_QUIT:
			m_sktSSock->AsyncSelect(FD_CLOSE);
		default:
			break;
		}
	}
	return LRESULT();
}

void CMySocketDlg::AddLog(CString cstrLog, CTime ctTime)
{
	UpdateData(TRUE);
	CString cache = ctTime.Format(_T("%Y/%m/%d %H:%M:%S "))+_T("\r\n") + cstrLog + _T("\r\n");
	m_cstrLog += cache;
	UpdateData(FALSE);

}

void CMySocketDlg::ClearLog()
{
	UpdateData(TRUE);
	m_cstrLog = "";
	UpdateData(FALSE);
}

void CMySocketDlg::AddContent(CString cstrVal,CString cstrUsrName,CTime ctTime)
{
	UpdateData(TRUE);
	CString cache = ctTime.Format(_T("%Y/%m/%d %H:%M:%S ")) + cstrUsrName + _T("\r\n") + cstrVal + _T("\r\n");
	m_cstrContent += cache;
	UpdateData(FALSE);
}

void CMySocketDlg::ClearContent()
{
	UpdateData(TRUE);
	m_cstrContent = "";
	UpdateData(FALSE);
}


void CMySocketDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_sktCSock->m_hSocket == INVALID_SOCKET)		//re-create valid socket
	{
		BOOL bFlag = m_sktCSock->Create(0, SOCK_STREAM, FD_CONNECT);
		if (!bFlag)
		{
			m_sktCSock->ProcErrorCode(GetLastError());
			m_sktCSock->Close();
			return;
		}
	}
	//m_sktCSock->Connect(m_cstrServerAddr, m_nPort);
	m_sktCSock->Connect(_T("223.3.217.186"), m_nPort);

	m_nTryCount++;
	if (m_nTryCount > 10 || m_sktCSock->IsConnected())
	{
		KillTimer(0);
		if (m_nTryCount > 10)
			AfxMessageBox(_T("Out of time!"));
		return;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMySocketDlg::OnBnClickedAbort()
{
	if (IsServer())
	{
		if (!m_sktSSock->m_lplistClients->IsEmpty())
		{
			Msg cache(_T("Server Quit."), 13, TP_QUIT, NOW_TIME);
			m_sktSSock->m_mdqMsgs->push_back(cache);
			m_sktSSock->EchoClients();
		}
		m_sktSSock->m_mdqMsgs->clear();
		m_sktSSock->m_mdqMsgs = NULL;
		m_sktSSock->m_lplistClients->RemoveAll();
		m_sktSSock->m_lplistClients = NULL;
		m_bIsServer = FALSE;
		m_sktSSock = NULL;
		m_btnConnect.EnableWindow(TRUE);
		m_btnHost.EnableWindow(TRUE);
	}
	else
	{

	}
}

void CMySocketDlg::OnBnClickedHost()
{
	
	if (m_sktSSock == NULL)
		m_sktSSock = new CSSock();
	//Clear socket
	//m_nPort = 1088;
	if (m_sktSSock->m_hSocket == INVALID_SOCKET)		//re-create valid socket
	{
		BOOL bFlag = m_sktSSock->Create(1088, SOCK_STREAM, FD_ACCEPT);
		if (!bFlag)
		{
			m_sktSSock->ProcErrorCode(GetLastError());
			m_sktSSock->Close();
			return;
		}
	}
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	m_cstrServerAddr = pApp->m_cstrHostIP;
	if (!m_sktSSock->Listen())
	{
		if (m_sktSSock->GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Error"));
			m_sktSSock->Close();
		}
	}
	m_bIsServer = TRUE;		//Become a server;
	AddLog(_T("Waiting for connecting... ") , NOW_TIME);
	//create and connect self-client 
	if (m_sktCSock == NULL)
		m_sktCSock = new CCSock();
	SetTimer(0, 1000, NULL);		//Set timer to connect
	m_nTryCount = 0;
	//m_sktCSock->Connect(pApp->m_cstrHostIP, m_nPort);
	//m_sktCSock->Connect("223.3.217.186", m_nPort);
	//Forbidden button
	m_btnConnect.EnableWindow(FALSE);
	m_btnHost.EnableWindow(FALSE);
}

void CMySocketDlg::OnBnClickedSend()
{
	if (m_sktCSock->IsConnected())
	{
		CString cache;
		m_edtTalk.GetWindowTextW(cache);
		Msg temp(cache.GetBuffer(), cache.GetLength() * sizeof(TCHAR), TP_TALK, NOW_TIME);
		m_sktCSock->m_nLength = sizeof(temp);
		m_sktCSock->FillBuffer(temp);
		m_sktCSock->AsyncSelect(FD_WRITE);
		m_edtTalk.SetWindowTextW(_T(""));
	}
	else
	{
		AfxMessageBox(_T(" No Connection."));
	}
}

void CMySocketDlg::OnBnClickedConnect()
{
	if (m_sktCSock == NULL)
	{
		AfxSocketInit();
		m_sktCSock = new CCSock();
	}
	/*m_sktSock->ShutDown(2);
	m_sktSock->m_hSocket = INVALID_SOCKET;
	m_sktSock->SetStatus(FALSE);*/
	//m_nPort = 1088;
	m_edtIP.GetWindowText(m_cstrServerAddr);	//Get target IP
	SetTimer(0, 1000, NULL);		//Set timer to connect
	m_nTryCount = 0;
}
