
// MySocket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MySocket.h"
#include "MySocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMySocketApp

BEGIN_MESSAGE_MAP(CMySocketApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMySocketApp construction

CMySocketApp::CMySocketApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMySocketApp object

CMySocketApp theApp;


// CMySocketApp initialization

BOOL CMySocketApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	memset(m_tszUser, 0, USER_LENGTH);
	GetLocalAddress(m_cstrHostIP);		//Get local IP
	/*CString test("test");
	char T[128];
	LPTSTR t = test.GetBuffer();
	memcpy(T,test.GetBuffer(),test.GetLength()*sizeof(TCHAR));*/
	CMySocketDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}
	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CMySocketApp::GetLocalAddress(CString &wszAdrr)
{
	TCHAR HostName[USER_LENGTH] = {'\0'};
	char t_hostname[USER_LENGTH] = {'\0'};
	//WideCharToMultiByte(CP_UTF8, 0, HostName, -1, t_hostname, WideCharToMultiByte(CP_UTF8, 0, HostName, -1, t_hostname, 0,NULL,NULL),NULL,NULL);
	gethostname(t_hostname, sizeof(HostName));// 获得本机主机名.
	UINT nBufferLength = MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, 0);
	MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, 0)); 
	//m_cstrUser = CString(HostName);
	memcpy(m_tszUser, HostName,nBufferLength*sizeof(TCHAR));
	hostent* hn;
	hn = gethostbyname(t_hostname);//根据本机主机名得到本机ip
	wszAdrr = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式
}







