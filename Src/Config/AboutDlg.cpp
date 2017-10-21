/*****************************************************************************
 AboutDlg.cpp
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
#include "AboutDlg.h"
#include "../Common/BuildTimestamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

CAboutDlg::CAboutDlg(CInstanceListDoc *pDoc, CWnd* pParent /*=NULL*/): 
	CDialog(CAboutDlg::IDD, pParent),
	pDocInstanceList(pDoc)
	{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_ABOUTWEBSITE, OnAboutHyperlink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
	{
	CDialog::OnInitDialog();
	
	SetWindowText("About " SZSERVICEDISPLAYNAME);
	SetDlgItemText(IDC_ABOUTAPPNAME, SZSERVICEDISPLAYNAME " " SZSERVICEVERSION);
	SetDlgItemText(IDC_ABOUTDATE, GetBuildDateTime());
	SetDlgItemText(IDC_ABOUTCOPYRIGHT, "Copyright (C) " SZCOPYRIGHTDATE " by " SZCOMPANYNAME);
	SetDlgItemText(IDC_ABOUTWEBSITE, SZWEBSITE);
	SetDlgItemText(IDC_ABOUTREGNAME, pDocInstanceList->cp.strRegName.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CAboutDlg::OnAboutHyperlink() 
	{
	WCHAR wszUrl[128] = {0};
	MultiByteToWideChar(CP_ACP, 0, "http://" SZWEBSITE, -1, wszUrl, 128);
	HlinkSimpleNavigateToString(wszUrl, NULL, NULL, NULL, 0, NULL, NULL, 0);
	}
