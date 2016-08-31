#include "stdafx.h"
#include "MySocket.h"
#include "CSock.h"
#include "BridgeSock.h"
#include "MySocketDlg.h"
//Server Socket
CSSock::CSSock()
{
	m_bConnected = false;
	m_nLength = 0;
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	m_mdqMsgs = new	MsgDeque();
	m_lplistClients = new CPtrList(5);
}

void CSSock::SetStatus(BOOL bStat)
{
	m_bConnected = bStat;
}

BOOL CSSock::IsConnected()
{
	return m_bConnected;
}

//TODO:
void CSSock::OnAccept(int nErrorCode)
{
	CBridgeSock *pSock = new CBridgeSock(this);		//create client pointer for possible connect
	if (Accept(*pSock))
	{
		m_lplistClients->AddTail(pSock);  //client en-queue
		CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		CString cstrCltName;
		UINT nCltPort;
		pSock->GetPeerName(cstrCltName,nCltPort);		//get information of client
		cstrCltName = _T("Connect to ") + cstrCltName;
		TCHAR cache[4096] = {'\0'};
		memcpy(cache, cstrCltName.GetBuffer(), cstrCltName.GetLength() * sizeof(TCHAR));
		//MultiByteToWideChar(CP_ACP, 0, cstrCltName.GetBuffer, -1, cache, MultiByteToWideChar(CP_ACP, 0, cstrCltName.GetBuffer, -1, cache, 0));
		Msg temp(cache, cstrCltName.GetLength()*sizeof(TCHAR) , TP_LOG, NOW_TIME);
		m_mdqMsgs->push_back(temp);
		EchoClients();
	}
	else
	{
		delete pSock;
	}
}



void CSSock::OnClose(int nErrorCode)
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_mdqMsgs->push_back(Msg(_T("Connection Broken."),19, TP_LOG, NOW_TIME));
		m_mdqMsgs->push_back(Msg(_T(" "),1,TP_QUIT, NOW_TIME));
		EchoClients();
		if (!m_mdqMsgs->empty())
			m_mdqMsgs->clear();
		if (!m_lplistClients->IsEmpty())
			m_lplistClients->RemoveAll();
		m_bConnected = FALSE;
		m_hSocket = INVALID_SOCKET;
		m_lplistClients = NULL;
		m_mdqMsgs = NULL;
		Close();
	}
	delete this;
}

CSSock::~CSSock()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}

void CSSock::ProcErrorCode(int nErrorCode)
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

void CSSock::EchoClients()
{
	while(!m_mdqMsgs->empty())
	{
		Msg tempMsg(m_mdqMsgs->front());
		POSITION ps = m_lplistClients->GetHeadPosition();
		while (ps != NULL)
		{
			CBridgeSock* tempBSock = (CBridgeSock*)m_lplistClients->GetNext(ps);
			tempBSock->FillBuffer(tempMsg);
			tempBSock->AsyncSelect(FD_WRITE);
		}
		m_mdqMsgs->pop_front();
	}
	
}


// CClintSock member functions
