#include "stdafx.h"
#include "SockMsg.h"


SockMsg::SockMsg()
{
	m_mValue.ctTime = GetCurrentTime();
	m_mValue.nType = 0;
	memset(m_mValue.szValue, 0, sizeof(m_mValue.szValue));
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

void SockMsg::Assign(int nType, TCHAR * szValue)
{
	Fresh();
	m_mValue.ctTime = NOW_TIME;
	m_mValue.nType = nType;
	lstrcpy(m_mValue.szValue, szValue);
}

BOOL SockMsg::InsertValue(UINT nType, TCHAR * szValue)
{
	if (szValue)
	{
		m_mValue.nType = nType;
		memcpy(m_mValue.szValue, szValue, sizeof(szValue));
		return 1;
	}
	return 0;
}

BOOL SockMsg::GetValue(TCHAR * szDst)
{
	if (szDst)
	{
		lstrcpy(m_mValue.szValue, szDst);
		return 1;
	}
	return 0;
}

BOOL SockMsg::IsEmpty()
{
	return m_mValue.nType == -1?FALSE:TRUE;
}

CString SockMsg::GetCString()
{
	CString cache(m_mValue.szValue);
	return cache;
}

int SockMsg::GetType()
{
	return m_mValue.nType;
}

CTime SockMsg::GetTime()
{
	return m_mValue.ctTime;
}
