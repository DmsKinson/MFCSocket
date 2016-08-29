#pragma once
#ifndef SOCK_MSG_H
#define SOCK_MSG_H

#define TP_TALK 0
#define TP_WUZIQI 1
#define TP_PAODEKUAI 2
#define TP_MINE 3
#define TP_LOG 4
#define TP_QUIT 5


struct Msg
{
	Msg(CString cstrValue, UINT nType, CTime ctTime);
	Msg(const Msg &msg);
	Msg();
	CTime ctTime;
	UINT nType;
	CString cstrValue;
};

class SockMsg
{
private:
	Msg m_mValue;
public:
	SockMsg();
	SockMsg(Msg smMsg);
	~SockMsg();
	void Fresh();
	void Assign(const Msg& src);
	//BOOL InsertValue(UINT nType,TCHAR *szValue);
	//BOOL GetValue(TCHAR *pszDst);
	BOOL IsEmpty();
	CString GetCString();
	int GetType();
	CTime GetTime();
};


#endif // !SOCK_MSG_H