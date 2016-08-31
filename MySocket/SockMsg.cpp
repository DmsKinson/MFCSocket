#include "stdafx.h"
#include "SockMsg.h"
#include "MySocket.h"

Msg::Msg(TCHAR* Value, UINT Length, UINT Type, CTime Time)
{
	memset(tszValue, 0, sizeof(tszValue));
	memcpy(tszValue, Value, Length);
	nType = Type;
	ctTime = Time;
}

Msg::Msg(const Msg & msg)
{
	memset(tszValue, 0, sizeof(tszValue));
	memcpy(this, &msg, sizeof(msg));
}

Msg::Msg()
{
	
}

SockMsg::SockMsg()
{
	m_mValue.ctTime = NOW_TIME;
	m_mValue.nType = 0;
	memset(m_mValue.tszValue, 0, sizeof(m_mValue.tszValue));
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
	memset(m_mValue.tszValue, 0, sizeof(m_mValue.tszValue));
	m_mValue.ctTime = CTime();
	m_mValue.nType = -1;
}

void SockMsg::Assign(const Msg & src)
{
	memcpy(&m_mValue, &src, sizeof(src));
}

BOOL SockMsg::IsEmpty()
{
	return m_mValue.nType<0;
}

CString SockMsg::GetCString()
{
	return m_mValue.tszValue;
}

int SockMsg::GetType()
{
	return m_mValue.nType;
}

CTime SockMsg::GetTime()
{
	return m_mValue.ctTime;
}
