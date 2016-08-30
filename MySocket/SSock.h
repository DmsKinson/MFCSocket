#pragma once
#ifndef SSOCK_H
#define SSOCK_H
//Server Socket
#include "SockMsg.h"
#include <deque>
using namespace std;
typedef deque<Msg> MsgDeque;
class CSSock : public CAsyncSocket
{
private:
	BOOL m_bConnected;
	Msg m_mBuffer;
public:
	UINT m_nLength;
	CPtrList *m_lplistClients;
	MsgDeque *m_mdqMsgs;
	CSSock();
	void SetStatus(BOOL stat);
	BOOL IsConnected();
	void ProcErrorCode(int nErrorCode);
	void EchoClients();
	//virtual void OnSend(int nErrorCode);
	//virtual void OnReceive(int nErrorCode);
	//virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	~CSSock();
};

#endif // !CSSOCK_H
