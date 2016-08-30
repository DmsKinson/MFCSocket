// BridgeSock.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"
#include "BridgeSock.h"
#include "MySocketDlg.h"

// BridgeSock

CBridgeSock::CBridgeSock()
{

}

CBridgeSock::CBridgeSock(CPtrList *lplistClients, MsgDeque *mdqMsgs)
{
	m_mBuffer.cstrValue.Empty();
	m_mBuffer.csterUser.Empty();
	m_lplistClients = lplistClients;
	m_mdqMsgs = mdqMsgs;
}

void CBridgeSock::FillBuffer(Msg & src)
{
	//memcpy(&m_mBuffer, &src, sizeof(src));
	m_mBuffer.cstrValue = src.cstrValue;
	m_mBuffer.ctTime = src.ctTime;
	m_mBuffer.nType = src.nType;
	m_nLength = sizeof(m_mBuffer);
}

void CBridgeSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	//pDlg->m_smMsg.Assign(m_mBuffer);
	m_mdqMsgs->push_back(Msg(m_mBuffer));
	//memcpy(&pDlg->m_smMsg, &m_mBuffer, sizeof(m_mBuffer));
	//::PostMessage(pDlg->handle, WM_USER_RECVMSG, 0, 0);
	//AsyncSelect(FD_WRITE);
	//CAsyncSocket::OnReceive(nErrorCode);
}

void CBridgeSock::OnSend(int nErrorCode)
{
	if (m_mBuffer.cstrValue.IsEmpty())
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
	//memset(&m_mBuffer, 0, sizeof(m_mBuffer));
	m_mBuffer.cstrValue.Empty();
	//AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

void CBridgeSock::OnClose(int nErrorCode)
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_hSocket = INVALID_SOCKET;
	}
}



CBridgeSock::~CBridgeSock()
{
	m_lplistClients = NULL;
	m_mdqMsgs = NULL;
}
