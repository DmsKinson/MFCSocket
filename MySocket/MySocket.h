
// MySocket.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMySocketApp:
// See MySocket.cpp for the implementation of this class
//

class CMySocketApp : public CWinApp
{
public:
	CMySocketApp();
	TCHAR m_tszUser[128];
	CString m_cstrHostIP;
// Overrides
public:
	virtual BOOL InitInstance();
	void GetLocalAddress(CString &wszAdrr);
// Implementation

	DECLARE_MESSAGE_MAP()
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CMySocketApp theApp;