#include "stdafx.h"
#include "MySocket.h"
#include "CSock.h"
#include "BridgeSock.h"
#include "MySocketDlg.h"
//Server Socket
CSSock::CSSock()
{
	m_bConnected = FALSE;
	m_nLength = 0;
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
		pSock->AsyncSelect(FD_READ);
		pSock->m_ssFather = this;
		m_lplistClients->AddTail(pSock);  //client en-queue
		CString cstrCltName;
		UINT nCltPort;
		pSock->GetPeerName(cstrCltName,nCltPort);		//get information of client
		cstrCltName = _T("Connect to ") + cstrCltName;
		Msg temp(cstrCltName.GetBuffer(),LOCAL_USER, cstrCltName.GetLength()*sizeof(TCHAR) , TP_LOG, NOW_TIME);
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
		m_mdqMsgs->push_back(Msg(_T("Connection Broken."),((CMySocketApp*)AfxGetApp())->m_tszUser,sizeof("Connection Broken.")*sizeof(TCHAR)+USER_LENGTH, TP_LOG, NOW_TIME));
		//m_mdqMsgs->push_back(Msg(_T(" "),2,TP_QUIT, NOW_TIME));
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
	//delete this;
}

CSSock::~CSSock()
{
	PreClose();
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

void CSSock::PreClose()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		if (!m_lplistClients->IsEmpty())
		{
			if (m_mdqMsgs != NULL)
			{
				Msg cache(_T("Connection interrupted!"), LOCAL_USER, sizeof(_T("Connection interrupted!")) * sizeof(TCHAR), TP_QUIT, NOW_TIME);
				m_mdqMsgs->push_back(cache);
				EchoClients();
				m_lplistClients = NULL;
				m_mdqMsgs = NULL;
				m_bConnected = FALSE;
				Close();
				m_hSocket = INVALID_SOCKET;
			}
		}
	}
	
}


// CClintSock member functions
