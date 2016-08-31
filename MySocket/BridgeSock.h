#pragma once
#pragma once
#ifndef BRIDGE_SOCK_H
#define BRIDGE_SOCK_H


#include "SockMsg.h"
#include "SSock.h"
#include <deque>
using namespace std;

typedef deque<Msg> MsgDeque;
class CBridgeSock :public CAsyncSocket
{
private:
	Msg m_mBuffer;
public:
	UINT m_nLength;
	CSSock *m_ssFather;
	CBridgeSock();
	CBridgeSock(CSSock *ssFather);
	void FillBuffer(Msg &src);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual ~CBridgeSock();
};


#endif // !BRIDGE_SOCK_H