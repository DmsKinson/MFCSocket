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

CBridgeSock::CBridgeSock(CSSock *ssFather)
{
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	m_ssFather = ssFather;
}

void CBridgeSock::PreClose()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_hSocket = INVALID_SOCKET;
	}
}

void CBridgeSock::FillBuffer(Msg & src)
{
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	memcpy(&m_mBuffer, &src, sizeof(src));
	m_nLength = sizeof(m_mBuffer);
}

void CBridgeSock::OnReceive(int nErrorCode)
{
	m_nLength = Receive(&m_mBuffer, sizeof(m_mBuffer));
	if (m_mBuffer.nType > 0)
	{
		if (m_mBuffer.nType == TP_QUIT)
		{
			m_ssFather->m_lplistClients->RemoveAt(m_ssFather->m_lplistClients->Find(this));
			m_mBuffer.nType = TP_LOG;
			m_ssFather->m_mdqMsgs->push_back(Msg(m_mBuffer));
			m_ssFather->EchoClients();
			m_ssFather = NULL;
			PreClose();
		}
		else
		{
			if (m_ssFather != NULL)
			{
				m_ssFather->m_mdqMsgs->push_back(Msg(m_mBuffer));
				m_ssFather->EchoClients();
				CAsyncSocket::OnReceive(nErrorCode);
			}
		}
		
	}
}

void CBridgeSock::OnSend(int nErrorCode)
{
	if (m_mBuffer.nType == 0)
	{
		AfxMessageBox(_T("Can't send empty messsage."));
		return;
	}
	Send(&m_mBuffer, m_nLength);
	m_nLength = 0;
	memset(m_mBuffer.tszValue, 0, sizeof(m_mBuffer.tszValue));
	m_mBuffer.nType = 0;
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

CBridgeSock::~CBridgeSock()
{
	m_ssFather = NULL;
}
