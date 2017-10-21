/*****************************************************************************
 TableDialog.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_TABLEDIALOG_H__B5E4A360_7B27_4FA1_B172_185313BF5394__INCLUDED_)
#define AFX_TABLEDIALOG_H__B5E4A360_7B27_4FA1_B172_185313BF5394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableDialog dialog

class CTableDialog : public CDialog
	{
	// Construction
	public:
	CTableDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CTableDialog)
	enum { IDD = IDD_TABLELIST };
	CListBox	m_ctlList;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	public:
	CString m_strSQLConnectionString;
	CString m_strTable;
	CString m_strColumn;
	DWORD m_dwColumnSize;
	protected:
	BOOL flSelectingColumn;
	CDatabase db;
	BOOL PrepareDialogData();

	// Generated message map functions
	//{{AFX_MSG(CTableDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClkList();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLEDIALOG_H__B5E4A360_7B27_4FA1_B172_185313BF5394__INCLUDED_)
