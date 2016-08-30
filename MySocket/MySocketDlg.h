
// MySocketDlg.h : header file
//

#pragma once
#ifndef MY_SOCKET_DLG_H
#define MY_SOCKET_DLG_H


#include "SockMsg.h"
#include "CSock.h"
#include "SSock.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMySocketDlg dialog
class CMySocketDlg : public CDialogEx
{
// Construction
private:
	int m_nTryCount;
	CCSock *m_sktCSock;
	CSSock *m_sktSSock;
	UINT m_nPort;
	BOOL m_bIsServer;
public:
	CMySocketDlg(CWnd* pParent = NULL);	// standard constructor
	SockMsg m_smMsg;
	CString m_cstrServerAddr;
	HWND handle;
	BOOL IsServer();
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
	CEdit m_edtTalk;
	CStatic m_csHostIP;
	CEdit m_edtIP;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedHost();
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);
	void AddLog(CString cstrLog,CTime ctTime);
	void ClearLog();
	void AddContent(CString cstrVal, CString cstrUsrName,CTime ctTime);
	void ClearContent();
	CString m_cstrLog;
	CString m_cstrContent;
	afx_msg void OnBnClickedAbort();
	CButton m_btnAbort;
	CButton m_btnConnect;
	CButton m_btnHost;
	CButton m_btnSend;
};

#endif // !MY_SOCKET_DLG_H