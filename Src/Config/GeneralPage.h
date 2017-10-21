/*****************************************************************************
 GeneralPage.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#if !defined(AFX_GENERALPAGE_H__91F52F52_24C0_4E8D_9C9D_D1BD1F6A0093__INCLUDED_)
#define AFX_GENERALPAGE_H__91F52F52_24C0_4E8D_9C9D_D1BD1F6A0093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
	{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	CGeneralPage();
	~CGeneralPage();

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_GENERALPAGE };
	CButton	m_ctlLogFilenameBtn;
	CEdit	m_ctlLogFilename;
	CComboBox	m_ctlPortName;
	CStatic	m_ctlLogRotationFile;
	CString	m_strPortName;
	CString	m_strBaudRate;
	CString	m_strLogFilename;
	int		m_nByteSize;
	int		m_nParity;
	int		m_nStopBits;
	BOOL	m_flLogConfig;
	BOOL	m_flLogStartStop;
	BOOL	m_flLogTimeouts;
	BOOL	m_flStartAutomatically;
	int		m_nLogRotation;
	int		m_nTimestampFormat;
	BOOL	m_flLogEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	const CInstanceParameters *ip;
	BOOL flMultipleSelection;
	void UpdateLogRotationFileFormat();

protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogFilenameBtn();
	afx_msg void OnLogOpen();
	afx_msg void OnChangeLogFilename();
	afx_msg void OnSelChangeLogRotation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__91F52F52_24C0_4E8D_9C9D_D1BD1F6A0093__INCLUDED_)
