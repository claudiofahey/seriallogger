/*****************************************************************************
 MainFrm.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_MAINFRM_H__52EBEC0C_45DD_4723_9212_2AC7FA879BF4__INCLUDED_)
#define AFX_MAINFRM_H__52EBEC0C_45DD_4723_9212_2AC7FA879BF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrame frame

class CMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
protected:
	CMainFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainFrame();

	// Generated message map functions
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__52EBEC0C_45DD_4723_9212_2AC7FA879BF4__INCLUDED_)
