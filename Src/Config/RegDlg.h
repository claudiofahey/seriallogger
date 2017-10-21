/*****************************************************************************
 RegDlg.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_REGDLG_H__ED762024_DA75_403B_944D_C0979CCD1C4F__INCLUDED_)
#define AFX_REGDLG_H__ED762024_DA75_403B_944D_C0979CCD1C4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegDlg dialog

class CRegDlg : public CDialog
	{
	// Construction
	public:
	CRegDlg(CInstanceListDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	CString m_strRegName;
	DWORD m_dwRegCode1;
	BOOL m_flValid;

	// Dialog Data
	//{{AFX_DATA(CRegDlg)
	enum { IDD = IDD_REGDLG };
	CEdit	c_RegName;
	CEdit	c_RegCode1;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	public:
	CInstanceListDoc *pDocInstanceList;
	protected:

	// Generated message map functions
	//{{AFX_MSG(CRegDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnValidate();
	afx_msg void OnRegDlgHyperlink();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGDLG_H__ED762024_DA75_403B_944D_C0979CCD1C4F__INCLUDED_)
