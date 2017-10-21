/*****************************************************************************
 TimeoutPage.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_TIMEOUTPAGE_H__BC279085_6F10_4C69_9F19_09F4F3711156__INCLUDED_)
#define AFX_TIMEOUTPAGE_H__BC279085_6F10_4C69_9F19_09F4F3711156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeoutPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTimeoutPage dialog

class CTimeoutPage : public CPropertyPage
	{
	DECLARE_DYNCREATE(CTimeoutPage)

// Construction
public:
	CTimeoutPage();
	~CTimeoutPage();

// Dialog Data
	//{{AFX_DATA(CTimeoutPage)
	enum { IDD = IDD_TIMEOUTPAGE };
	CString	m_strTimeoutSec;
	CString	m_strWarningIntervalSec;
	BOOL	m_flSunday;
	int		m_iTimeoutSecGrp;
	BOOL	m_flEntireDay;
	BOOL	m_flFriday;
	BOOL	m_flMonday;
	CString	m_strNetSendList;
	BOOL	m_flSaturday;
	BOOL	m_flThursday;
	CString	m_strTimeoutCmd;
	BOOL	m_flTuesday;
	BOOL	m_flWednesday;
	CTime	m_timeBegin;
	CTime	m_timeEnd;
	int		m_iWarningIntervalGrp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTimeoutPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableFields();
	// Generated message map functions
	//{{AFX_MSG(CTimeoutPage)
	afx_msg void OnNeverTimeout();
	afx_msg void OnAllowTimeout();
	afx_msg void OnEntireDay();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimeoutCmdBrowseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEOUTPAGE_H__BC279085_6F10_4C69_9F19_09F4F3711156__INCLUDED_)
