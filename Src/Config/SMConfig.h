/*****************************************************************************
 SMConfig.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_SMCONFIG_H__6A8B0C36_1493_42C8_9A4A_C3247920CD2E__INCLUDED_)
#define AFX_SMCONFIG_H__6A8B0C36_1493_42C8_9A4A_C3247920CD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CSMConfigApp:
// See SMConfig.cpp for the implementation of this class
//

class CSMConfigApp : public CWinApp
	{
	public:
	CSMConfigApp();
	int DisplayError(LPCTSTR lpszText);
	BOOL RemoveAll();

	CString m_strHelpFilename;

	void SetStatusText(LPCTSTR lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMConfigApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
	protected:

	//{{AFX_MSG(CSMConfigApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMCONFIG_H__6A8B0C36_1493_42C8_9A4A_C3247920CD2E__INCLUDED_)
