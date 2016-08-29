// ClintSock.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"
#include "CSock.h"
#include "MySocketDlg.h"


// CClintSock

CSock::CSock()
{
	m_bConnected = false;
	m_nLength = 0;
	memset(&m_mBuffer, 0, sizeof(m_mBuffer));
}

void CSock::SetStatus(BOOL bStat)
{
	m_bConnected = bStat;
}

BOOL CSock::IsConnected()
{
	return m_bConnected;
}

void CSock::OnSend(int nErrorCode)
{
	Send(&m_mBuffer, m_nLength);
	m_nLength = 0;
	memset(&m_mBuffer, 0, sizeof(m_mBuffer));
	AsyncSelect(FD_READ);
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	pDlg->AddContent(m_mBuffer.szValue, _T("Husky"), NOW_TIME);
	CAsyncSocket::OnSend(nErrorCode);
}

void CSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	pDlg->m_smMsg.Assign(m_mBuffer);
	//memcpy(&pDlg->m_smMsg, &m_mBuffer, sizeof(m_mBuffer));
	::PostMessage(pDlg->handle, WM_USER_RECVMSG, 0, 0);
	//AsyncSelect(FD_WRITE);
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSock::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		m_bConnected = TRUE;
		CMySocketApp *pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg *pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		CString cache(pDlg->m_szServerAddr);
		pDlg->AddLog(_T("Connect to ") + cache, NOW_TIME);
		//memcpy(m_mBuffer.szValue, "Connect to", 11);
		//lstrcat(m_mBuffer.szValue, pDlg->m_szServerAddr);
		//m_mBuffer.ctTime = CTime::GetCurrentTime();
		//m_mBuffer.nType = TP_SYS;
		//pDlg->m_smMsg.Assign(m_mBuffer);
		//AsyncSelect(FD_WRITE);
	}
	else
		ProcErrorCode(nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
}

void CSock::OnAccept(int nErrorCode)
{
	CSock *pSock = new CSock();
	if (Accept(*pSock))
	{
		//AfxMessageBox(_T("get accept"));
		pSock->AsyncSelect(FD_READ);
		m_sktCltSock = pSock;
		CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		CString cstrCltName;
		UINT nCltPort;
		pSock->GetPeerName(cstrCltName,nCltPort);
		pDlg->AddLog(_T("Connect to ") + cstrCltName, NOW_TIME);
	}
	else
	{
		delete pSock;
	}
}



void CSock::OnClose(int nErrorCode)
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_bConnected = FALSE;
		Close();
	}
	delete this;
}

CSock::~CSock()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}

void CSock::ProcErrorCode(int nErrorCode)
{
	switch (nErrorCode)
	{
	case WSANOTINITIALISED:
		AfxMessageBox(_T("A successful AfxSocketInit must occur before using this API."));
		break;
	case WSAENETDOWN:
		AfxMessageBox(_T("The Windows Sockets implementation detected that the network subsystem failed."));
		break;
	case WSAEAFNOSUPPORT:
		AfxMessageBox(_T("The specified address family is not supported."));
		break;
	case WSAEINPROGRESS:
		AfxMessageBox(_T("A blocking Windows Sockets operation is in progress."));
		break;
	case WSAEMFILE:
		AfxMessageBox(_T("No more file descriptors are available."));
		break;
	case WSAENOBUFS:
		AfxMessageBox(_T("No buffer space is available. The socket cannot be created."));
		break;
	case WSAEPROTONOSUPPORT:
		AfxMessageBox(_T("The specified port is not supported."));
		break;
	case WSAEPROTOTYPE:
		AfxMessageBox(_T("The specified port is the wrong type for this socket."));
		break;
	case WSAESOCKTNOSUPPORT:
		AfxMessageBox(_T("The specified socket type is not supported in this address family."));
		break;
	case 10048:
		AfxMessageBox(_T("Port is occupied."));
		break;
	default:
		AfxMessageBox(_T("Unknown Error."));
		break;
	}
}


// CClintSock member functions
