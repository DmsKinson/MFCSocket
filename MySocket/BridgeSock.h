#pragma once
#pragma once
#ifndef BRIDGE_SOCK_H
#define BRIDGE_SOCK_H


#include "SockMsg.h"
#include <deque>
using namespace std;

typedef deque<Msg> MsgDeque;
class CBridgeSock :public CAsyncSocket
{
private:
	Msg m_mBuffer;
public:
	UINT m_nLength;
	CPtrList *m_lplistClients;
	MsgDeque *m_mdqMsgs;
	CBridgeSock();
	CBridgeSock(CPtrList *lplistClients, MsgDeque *mdqMsgs);
	void FillBuffer(Msg &src);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual ~CBridgeSock();
};


#endif // !BRIDGE_SOCK_H