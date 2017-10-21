/*****************************************************************************
 OdbcPage.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_ODBCPAGE_H__60CC9767_9FCE_44D8_853C_025B120151A9__INCLUDED_)
#define AFX_ODBCPAGE_H__60CC9767_9FCE_44D8_853C_025B120151A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COdbcPage dialog

class COdbcPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COdbcPage)

// Construction
public:
	COdbcPage();
	~COdbcPage();

// Dialog Data
	//{{AFX_DATA(COdbcPage)
	enum { IDD = IDD_ODBCPAGE };
	CEdit	m_ctlMaxLineLength;
	CEdit	m_ctlSQLWriteLineStatement;
	CEdit	m_ctlSQLSafeConnectionString;
	CString	m_strSQLWriteLineStatement;
	UINT	m_dwMaxLineLength;
	CString	m_strSQLSafeConnectionString;
	//}}AFX_DATA
	CString	m_strSQLConnectionString;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COdbcPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COdbcPage)
	afx_msg void OnBrowseDsn();
	afx_msg void OnChangeConnectString();
	afx_msg void OnBuildSqlStatement();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCPAGE_H__60CC9767_9FCE_44D8_853C_025B120151A9__INCLUDED_)
