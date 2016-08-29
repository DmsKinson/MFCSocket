#include "stdafx.h"
#include "SockMsg.h"

Msg::Msg(CString Value, UINT Type, CTime Time)
{
	cstrValue = Value;
	nType = Type;
	ctTime = Time;
}

Msg::Msg(const Msg & msg)
{
	//memcpy(this, &msg, sizeof(msg));
	cstrValue = msg.cstrValue;
	nType = msg.nType;
	ctTime = msg.ctTime;
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
	memset(&m_mValue, 0, sizeof(m_mValue));
}

void SockMsg::Assign(const Msg & src)
{
	memcpy(&m_mValue, &src, sizeof(src));
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
