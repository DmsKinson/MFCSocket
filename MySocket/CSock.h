#pragma once
#ifndef CSOCK_H
#define CSOCK_H
#include "SockMsg.h"
//client socket
class CCSock : public CAsyncSocket
{
private:
	BOOL m_bConnected;
public:
	UINT m_nLength;
	Msg m_mBuffer;
	//CPtrList m_lplistClients;
	CCSock();
	void SetStatus(BOOL stat);
	BOOL IsConnected();
	void FillBuffer(Msg &src);
	void ProcErrorCode(int nErrorCode);
	void PreClose();
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual ~CCSock();
};
#endif // !CCSOCK_H
