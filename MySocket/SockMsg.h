#pragma once
#ifndef SOCK_MSG_H
#define SOCK_MSG_H

#define USER_LENGTH 128
#define TP_TALK 6
#define TP_WUZIQI 1
#define TP_PAODEKUAI 2
#define TP_MINE 3
#define TP_LOG 4
#define TP_QUIT 5


struct Msg
{
	Msg(TCHAR* Value,TCHAR* User, UINT Length, UINT Type, CTime Time);
	Msg(const Msg &msg);
	Msg();
	//TCHAR *tszUser;
	CTime ctTime;
	UINT nType;
	TCHAR tszValue[4096];
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
	BOOL IsEmpty();
	CString GetCString();
	CString GetUser();
	int GetType();
	CTime GetTime();
};

#endif // !SOCK_MSG_H