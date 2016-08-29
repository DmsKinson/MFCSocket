#pragma once
#ifndef CSOCK_H
#define CSOCK_H


// CClintSock command
#include "SockMsg.h"
class CSock : public CAsyncSocket
{
private:
	BOOL m_bConnected;
	Msg m_mBuffer;
public:
	CSock *m_sktCltSock;
	UINT m_nLength;
	CSock();
	void SetStatus(BOOL stat);
	BOOL IsConnected();
	void ProcErrorCode(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual ~CSock();
};

#endif // !CLINT_SOCK_H
