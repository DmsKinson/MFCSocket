
// MySocketDlg.h : header file
//

#pragma once
#ifndef MY_SOCKET_DLG_H
#define MY_SOCKET_DLG_H


#include "SockMsg.h"
#include "CSock.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMySocketDlg dialog
class CMySocketDlg : public CDialogEx
{
// Construction
private:
	int m_nTryCount;
	CSock m_sktSock;
	UINT m_nPort;
public:
	CMySocketDlg(CWnd* pParent = NULL);	// standard constructor
	SockMsg m_smMsg;
	TCHAR m_szServerAddr[256];
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedConnect();
	CEdit m_edtIP;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSend();
	CEdit m_edtTalk;
	CStatic m_csHostIP;
};

#endif // !MY_SOCKET_DLG_H