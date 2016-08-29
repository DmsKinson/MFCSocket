#pragma once
#ifndef SOCK_MSG_H
#define SOCK_MSG_H

#define TP_TALK 0
#define TP_WUZIQI 1
#define TP_PAODEKUAI 2
#define TP_MINE 3
#define TP_SYS 4


struct Msg
{
	CTime ctTime;
	UINT nType;
	TCHAR szValue[4096];
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
	void Assign(int nType, TCHAR *szValue);
	BOOL InsertValue(UINT nType,TCHAR *szValue);
	BOOL GetValue(TCHAR *pszDst);
	BOOL IsEmpty();
	CString GetCString();
	int GetType();
	CTime GetTime();
};


#endif // !SOCK_MSG_H