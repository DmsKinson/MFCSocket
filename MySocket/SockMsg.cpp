#include "stdafx.h"
#include "SockMsg.h"
#include "MySocket.h"

Msg::Msg(CString Value, UINT Type, CTime Time)
{
	cstrValue = Value;
	nType = Type;
	ctTime = Time;
	csterUser = ((CMySocketApp*)AfxGetApp())->m_cstrUser;
}

Msg::Msg(const Msg & msg)
{
	//memcpy(this, &msg, sizeof(msg));
	cstrValue = msg.cstrValue;
	nType = msg.nType;
	ctTime = msg.ctTime;
	csterUser = ((CMySocketApp*)AfxGetApp())->m_cstrUser;
}

Msg::Msg()
{
	
}

SockMsg::SockMsg()
{
	m_mValue.ctTime = NOW_TIME;
	m_mValue.nType = 0;
	m_mValue.cstrValue.Empty();
}

SockMsg::SockMsg(Msg smMsg)
{
	Fresh();
	Assign(smMsg);
}


SockMsg::~SockMsg()
{
}

void SockMsg::Fresh()
{
	m_mValue.csterUser.Empty();
	m_mValue.cstrValue.Empty();
	m_mValue.ctTime = CTime();
	m_mValue.nType = 0;
}

void SockMsg::Assign(const Msg & src)
{
	//memcpy(&m_mValue, &src, sizeof(src));
	m_mValue.csterUser = src.csterUser;
	m_mValue.cstrValue = src.cstrValue;
	m_mValue.ctTime = src.ctTime;
	m_mValue.nType = src.nType;
}

//BOOL SockMsg::InsertValue(UINT nType, TCHAR * szValue)
//{
//	if (szValue)
//	{
//		m_mValue.nType = nType;
//		memcpy(m_mValue.cstrValue, szValue, sizeof(szValue));
//		return 1;
//	}
//	return 0;
//}
//
//BOOL SockMsg::GetValue(TCHAR * szDst)
//{
//	if (szDst)
//	{
//		lstrcpy(m_mValue.cstrValue, szDst);
//		return 1;
//	}
//	return 0;
//}

BOOL SockMsg::IsEmpty()
{
	return m_mValue.cstrValue.IsEmpty();
}

CString SockMsg::GetCString()
{
	return m_mValue.cstrValue;
}

int SockMsg::GetType()
{
	return m_mValue.nType;
}

CTime SockMsg::GetTime()
{
	return m_mValue.ctTime;
}
