#include "stdafx.h"
#include "MySocket.h"
#include "CSock.h"
#include "MySocketDlg.h"
//client socket
CCSock::CCSock()
{
	m_bConnected = false;
	m_nLength = 0;
	memset(&m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	//m_mBuffer.csterUser.Empty();
	//m_mBuffer.tszValue.Empty();
}

void CCSock::SetStatus(BOOL bStat)
{
	m_bConnected = bStat;
}

BOOL CCSock::IsConnected()
{
	return m_bConnected;
}

void CCSock::FillBuffer(Msg & src)
{
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	memcpy(&m_mBuffer, &src, sizeof(src));
	/*m_mBuffer.tszValue = src.tszValue;
	m_mBuffer.ctTime = src.ctTime;
	m_mBuffer.nType = src.nType;*/
	m_nLength = sizeof(m_mBuffer);
}


void CCSock::OnSend(int nErrorCode)
{
	if (m_mBuffer.nType < 0)
	{
		AfxMessageBox(_T("Can't send empty messsage."));
		return;
	}
	Send(&m_mBuffer, m_nLength);
	//AsyncSelect(FD_READ);
	//CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	//CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	//pDlg->AddContent(m_mBuffer.cstrValue, _T("Husky"), NOW_TIME);
	m_nLength = 0;
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	//m_mBuffer.tszValue.Empty();
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

void CCSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	if (m_nLength > 0)
	{
		CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		pDlg->m_smMsg.Assign(m_mBuffer);
		//memcpy(&pDlg->m_smMsg, &m_mBuffer, sizeof(m_mBuffer));
		::PostMessage(pDlg->handle, WM_USER_RECVMSG, 0, 0);
		CAsyncSocket::OnReceive(nErrorCode);
	}
}

void CCSock::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
		{
			m_bConnected = TRUE;
			CMySocketApp *pApp = (CMySocketApp*)AfxGetApp();
			CMySocketDlg *pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
			AsyncSelect(FD_READ);
		}
		else
			ProcErrorCode(nErrorCode);
		CAsyncSocket::OnConnect(nErrorCode);
}

void CCSock::OnClose(int nErrorCode)
{

	if (m_hSocket != INVALID_SOCKET)
	{
		m_bConnected = FALSE;
		m_hSocket = INVALID_SOCKET;
		Close();
	}
	delete this;
}

CCSock::~CCSock()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}

void CCSock::ProcErrorCode(int nErrorCode)
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