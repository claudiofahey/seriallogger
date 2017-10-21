/*****************************************************************************
 TimeoutPage.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "SMConfig.h"
#include "TimeoutPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeoutPage property page

IMPLEMENT_DYNCREATE(CTimeoutPage, CPropertyPage)

CTimeoutPage::CTimeoutPage() : CPropertyPage(CTimeoutPage::IDD)
	{
	//{{AFX_DATA_INIT(CTimeoutPage)
	m_strTimeoutSec = _T("");
	m_strWarningIntervalSec = _T("");
	m_flSunday = FALSE;
	m_iTimeoutSecGrp = -1;
	m_flEntireDay = FALSE;
	m_flFriday = FALSE;
	m_flMonday = FALSE;
	m_strNetSendList = _T("");
	m_flSaturday = FALSE;
	m_flThursday = FALSE;
	m_strTimeoutCmd = _T("");
	m_flTuesday = FALSE;
	m_flWednesday = FALSE;
	m_timeBegin = 0;
	m_timeEnd = 0;
	m_iWarningIntervalGrp = -1;
	//}}AFX_DATA_INIT
	}

CTimeoutPage::~CTimeoutPage()
	{
	}

void CTimeoutPage::DoDataExchange(CDataExchange* pDX)
	{
	//TRACE0("CTimeoutPage::DoDataExchange\n");
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeoutPage)
	DDX_Text(pDX, IDC_TIMEOUTSEC, m_strTimeoutSec);
	DDX_Text(pDX, IDC_WARNINGINTERVALSEC, m_strWarningIntervalSec);
	DDX_Check(pDX, IDC_SUNDAY, m_flSunday);
	DDX_Radio(pDX, IDC_NEVERTIMEOUT, m_iTimeoutSecGrp);
	DDX_Check(pDX, IDC_ENTIREDAY, m_flEntireDay);
	DDX_Check(pDX, IDC_FRIDAY, m_flFriday);
	DDX_Check(pDX, IDC_MONDAY, m_flMonday);
	DDX_Text(pDX, IDC_NETSENDLIST, m_strNetSendList);
	DDX_Check(pDX, IDC_SATURDAY, m_flSaturday);
	DDX_Check(pDX, IDC_THURSDAY, m_flThursday);
	DDX_Text(pDX, IDC_TIMEOUTCMD, m_strTimeoutCmd);
	DDX_Check(pDX, IDC_TUESDAY, m_flTuesday);
	DDX_Check(pDX, IDC_WEDNESDAY, m_flWednesday);
	DDX_DateTimeCtrl(pDX, IDC_TIMEBEGIN, m_timeBegin);
	DDX_DateTimeCtrl(pDX, IDC_TIMEEND, m_timeEnd);
	DDX_Radio(pDX, IDC_WARNINGINTERVALONCE, m_iWarningIntervalGrp);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CTimeoutPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTimeoutPage)
	ON_BN_CLICKED(IDC_NEVERTIMEOUT, OnNeverTimeout)
	ON_BN_CLICKED(IDC_ALLOWTIMEOUT, OnAllowTimeout)
	ON_BN_CLICKED(IDC_ENTIREDAY, OnEntireDay)
	ON_BN_CLICKED(IDC_TIMEOUTCMDBROWSEBTN, OnTimeoutCmdBrowseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeoutPage message handlers

void CTimeoutPage::OnNeverTimeout() 
	{
	//TRACE0("CTimeoutPage::OnNeverTimeout\n");
	EnableFields();
	}

void CTimeoutPage::OnAllowTimeout() 
	{
	//TRACE0("CTimeoutPage::OnAllowTimeout\n");
	EnableFields();
	}

void CTimeoutPage::OnEntireDay() 
	{
	EnableFields();
	}

void CTimeoutPage::EnableFields()
	{
	//TRACE0("CTimeoutPage::EnableFields\n");

	BOOL flAllowTimeout = IsDlgButtonChecked(IDC_ALLOWTIMEOUT);
	BOOL flEntireDay = IsDlgButtonChecked(IDC_ENTIREDAY);

	GetDlgItem(IDC_WARNINGINTERVALONCE)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_WARNINGINTERVALREPEAT)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_WARNINGINTERVALSEC)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_WARNINGINTERVALTEXT1)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEOUTDAYSTEXT1)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_SUNDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_MONDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TUESDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_WEDNESDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_THURSDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_FRIDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_SATURDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEOUTTIMESTEXT1)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_ENTIREDAY)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEBEGIN)->EnableWindow(flAllowTimeout && !flEntireDay);
	GetDlgItem(IDC_TIMEOUTTIMESTEXT2)->EnableWindow(flAllowTimeout && !flEntireDay);
	GetDlgItem(IDC_TIMEEND)->EnableWindow(flAllowTimeout && !flEntireDay);
	GetDlgItem(IDC_TIMEOUTCMDTEXT1)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEOUTCMD)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEOUTCMDBROWSEBTN)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_TIMEOUTCMDTEXT2)->EnableWindow(flAllowTimeout);
	GetDlgItem(IDC_NETSENDLIST)->EnableWindow(flAllowTimeout);
	}

BOOL CTimeoutPage::OnInitDialog() 
	{
	CPropertyPage::OnInitDialog();	// this calls UpdateData()
	
	EnableFields();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CTimeoutPage::OnTimeoutCmdBrowseBtn() 
	{
	CString strInitialFilename;
	GetDlgItemText(IDC_TIMEOUTCMD, strInitialFilename);

	char szFilter[] = "All Files (*.*)|*.*||";

	CFileDialog fd(TRUE, NULL, strInitialFilename, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	int iRc = fd.DoModal();
	if (iRc == IDCANCEL && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
		{
		fd.m_ofn.lpstrFile[0] = 0;
		iRc = fd.DoModal();
		}
	if (iRc == IDOK)
		SetDlgItemText(IDC_TIMEOUTCMD, fd.GetPathName());	
	}
