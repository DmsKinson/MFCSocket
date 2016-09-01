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
public:
	UINT m_nLength;
	CPtrList *m_lplistClients;
	MsgDeque *m_mdqMsgs;
	CSSock();
	void SetStatus(BOOL stat);
	BOOL IsConnected();
	void ProcErrorCode(int nErrorCode);
	void EchoClients();
	void PreClose();	
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	~CSSock();
};

#endif // !CSSOCK_H
