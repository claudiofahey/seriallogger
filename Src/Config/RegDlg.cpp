/*****************************************************************************
 RegDlg.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "SMConfig.h"
//#include "MainWnd.h"
#include "InstanceListDoc.h"
#include "RegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegDlg dialog

CRegDlg::CRegDlg(CInstanceListDoc *pDoc, CWnd* pParent /*=NULL*/): 
	CDialog(CRegDlg::IDD, pParent),
	m_dwRegCode1(0),
	m_flValid(0),
	pDocInstanceList(pDoc)
	{
	//{{AFX_DATA_INIT(CRegDlg)
	//}}AFX_DATA_INIT
	}	

void CRegDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegDlg)
	DDX_Control(pDX, IDC_REGDLGNAME, c_RegName);
	DDX_Control(pDX, IDC_REGDLGCODE, c_RegCode1);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CRegDlg, CDialog)
	//{{AFX_MSG_MAP(CRegDlg)
	ON_BN_CLICKED(IDC_REGDLGVALIDATE, OnValidate)
	ON_BN_CLICKED(IDC_REGDLGWEBSITE, OnRegDlgHyperlink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegDlg message handlers

BOOL CRegDlg::OnInitDialog() 
	{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_REGDLGWEBSITE, SZWEBSITE);
	SetDlgItemText(IDC_REGDLGCURREGNAME, pDocInstanceList->cp.strRegName.c_str());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CRegDlg::OnValidate()
	{
	CString strRegCode1;
	c_RegName.GetWindowText(m_strRegName);
	c_RegCode1.GetWindowText(strRegCode1);
	m_dwRegCode1 = strtoul(strRegCode1, 0, 16);
	//TRACE2("CRegDlg::OnValidate: %s, %08x\n", (LPCSTR) m_strRegName, m_dwRegCode1);

	if (ValidateRegCode(m_strRegName, m_dwRegCode1))
		{
		m_flValid = 1;
		c_RegName.EnableWindow(FALSE);
		c_RegCode1.EnableWindow(FALSE);
		MessageBox("The registration code is valid.  You will need to restart the service "
			"in order for changes to take effect.","Registration Successful",
			MB_ICONINFORMATION);
		}
	else
		{
		m_flValid = 0;
		MessageBox("The registration code you entered does not correspond to the registration name.  "
			"You must enter the name and code exactly as provided to you.", "Invalid Registration Code",
			MB_ICONERROR);
		}
	}

void CRegDlg::OnRegDlgHyperlink() 
	{
	WCHAR wszUrl[128] = {0};
	MultiByteToWideChar(CP_ACP, 0, "http://" SZWEBSITE, -1, wszUrl, 128);
	HlinkSimpleNavigateToString(wszUrl, NULL, NULL, NULL, 0, NULL, NULL, 0);
	}

void CRegDlg::OnOK()
	{
	// TODO: Add extra validation here
	if (!m_flValid && (c_RegName.GetWindowTextLength() || c_RegCode1.GetWindowTextLength()))
		{
		int iRc = MessageBox("You have not successfully validated the registration code.  Are you "
			"sure you want to close this window without validating the code?  To validate the code "
			"click the Validate button.", "Registration Code Not Validated",
			MB_ICONQUESTION | MB_YESNOCANCEL);
		if (iRc != IDYES)
			return;
		}
	
	CDialog::OnOK();
	}
