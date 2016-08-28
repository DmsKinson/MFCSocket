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
	CAsyncSocket::OnSend(nErrorCode);
}

void CSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	CMySocketDlg* pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
	pDlg->m_smMsg.Assign(m_mBuffer);
	//memcpy(&pDlg->m_smMsg, &m_mBuffer, sizeof(m_mBuffer));
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSock::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		m_bConnected = TRUE;
		CMySocketApp *pApp = (CMySocketApp*)AfxGetApp();
		CMySocketDlg *pDlg = (CMySocketDlg*)pApp->m_pMainWnd;
		memcpy(m_mBuffer.szValue, "Connect to", 11);
		lstrcat(m_mBuffer.szValue, pDlg->m_szServerAddr);
		m_mBuffer.ctTime = CTime::GetCurrentTime();
		m_mBuffer.nType = TP_SYS;
		pDlg->m_smMsg.Assign(m_mBuffer);
		AsyncSelect(FD_READ);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

CSock::~CSock()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}


// CClintSock member functions
