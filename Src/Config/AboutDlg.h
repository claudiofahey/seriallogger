/*****************************************************************************
 AboutDlg.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_ABOUTDLG_H__ACA7B629_3D81_4E7F_9A44_7956D79DF91A__INCLUDED_)
#define AFX_ABOUTDLG_H__ACA7B629_3D81_4E7F_9A44_7956D79DF91A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
	{
	// Construction
	public:
	CAboutDlg(CInstanceListDoc *pDoc, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	public:
	CInstanceListDoc *pDocInstanceList;
	protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAboutHyperlink();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__ACA7B629_3D81_4E7F_9A44_7956D79DF91A__INCLUDED_)
