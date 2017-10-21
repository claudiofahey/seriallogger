/*****************************************************************************
 EmailPage.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_EMAILPAGE_H__FA02D4B5_109A_437F_9990_6C45191C5822__INCLUDED_)
#define AFX_EMAILPAGE_H__FA02D4B5_109A_437F_9990_6C45191C5822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmailPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmailPage dialog

class CEmailPage : public CPropertyPage
	{
	DECLARE_DYNCREATE(CEmailPage)

	// Construction
	public:
	CEmailPage();
	~CEmailPage();

	// Dialog Data
	//{{AFX_DATA(CEmailPage)
	enum { IDD = IDD_EMAILPAGE };
	CString	m_strSmtpFromAddress;
	CString	m_strSmtpServer;
	CString	m_strSmtpToAddress;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEmailPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	public:
	const CInstanceParameters *ip;
	CGeneralPage *pGeneralPage;
	protected:
	// Generated message map functions
	//{{AFX_MSG(CEmailPage)
	afx_msg void OnSendNow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILPAGE_H__FA02D4B5_109A_437F_9990_6C45191C5822__INCLUDED_)
