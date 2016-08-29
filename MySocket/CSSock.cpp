#include "stdafx.h"
#include "MySocket.h"
#include "CSSock.h"
#include "MySocketDlg.h"
//Server Socket
CSSock::CSSock()
{
	m_bConnected = false;
	m_nLength = 0;
	memset(&m_mBuffer, 0, sizeof(m_mBuffer));
}

void CSSock::SetStatus(BOOL bStat)
{
	m_bConnected = bStat;
}

BOOL CSSock::IsConnected()
{
	return m_bConnected;
}

//void CCSock::OnSend(int nErrorCode)
//{
//	for (MsgDeque::iterator iter = m_mdqMsgs.begin(); iter != m_mdqMsgs.end(); iter++)
//	{
//		Send(&m_mdqMsgs.front(), sizeof(m_mdqMsgs.front()));
//		m_mdqMsgs.pop_front();
//	}
//	//m_nLength = 0;
//	//memset(&m_mBuffer, 0, sizeof(m_mBuffer));
//
//	//AsyncSelect(FD_READ);
//	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
//	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
//	//pDlg->AddContent(m_mBuffer.cstrValue, _T("Husky"), NOW_TIME);
//	CAsyncSocket::OnSend(nErrorCode);
//}

void CSSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	//pDlg->m_smMsg.Assign(m_mBuffer);
	m_mdqMsgs.push_back(Msg(m_mBuffer));
	//memcpy(&pDlg->m_smMsg, &m_mBuffer, sizeof(m_mBuffer));
	//::PostMessage(pDlg->handle, WM_USER_RECVMSG, 0, 0);
	//AsyncSelect(FD_WRITE);
	//CAsyncSocket::OnReceive(nErrorCode);
	EchoClients();
}


//TODO:
void CSSock::OnAccept(int nErrorCode)
{
	CCSock *pSock = new CCSock();		//create client pointer for possible connect
	if (Accept(*pSock))
	{
		m_lplistClients.AddTail(pSock);
		CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		CString cstrCltName;
		UINT nCltPort;
		pSock->GetPeerName(cstrCltName,nCltPort);		//get information of client
		m_mdqMsgs.push_back(Msg(_T("Connect to" + cstrCltName),TP_LOG,NOW_TIME));
		//pDlg->AddLog(_T("Connect to ") + cstrCltName, NOW_TIME);
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
		m_bConnected = FALSE;
		m_hSocket = INVALID_SOCKET;
		m_mdqMsgs.push_back(Msg(_T("Connection Broken."), TP_LOG, NOW_TIME));
		m_mdqMsgs.push_back(Msg(_T(""),TP_QUIT, NOW_TIME));
		EchoClients();
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
	for (MsgDeque::iterator iter = m_mdqMsgs.begin(); iter != m_mdqMsgs.end(); iter++)
	{
		Msg tempMsg(*iter);
		POSITION ps = m_lplistClients.GetHeadPosition();
		while (ps != NULL)
		{
			CCSock* tempCCSock = (CCSock*)m_lplistClients.GetNext(ps);
			memcpy(&tempCCSock->m_mBuffer, &tempMsg,sizeof(m_mdqMsgs.front()));
			/*tempCCSock->m_mBuffer.cstrValue = tempMsg.cstrValue;
			tempCCSock->m_mBuffer.ctTime = tempMsg.ctTime;
			tempCCSock->m_mBuffer.nType = tempMsg.nType;*/
			tempCCSock->AsyncSelect(FD_WRITE);
		}
		m_mdqMsgs.pop_front();		
	}
	
}


// CClintSock member functions
