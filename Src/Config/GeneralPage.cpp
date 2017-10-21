/*****************************************************************************
 GeneralPage.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "SMConfig.h"
//#include "MainWnd.h"
#include "GeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : 
	CPropertyPage(CGeneralPage::IDD),
	ip(0),
	flMultipleSelection(0)
	{
	//{{AFX_DATA_INIT(CGeneralPage)
	m_strPortName = _T("COM1");
	m_strBaudRate = _T("2400");
	m_strLogFilename = _T("");
	m_nByteSize = 4;
	m_nParity = 0;
	m_nStopBits = 0;
	m_flLogConfig = FALSE;
	m_flLogStartStop = FALSE;
	m_flLogTimeouts = FALSE;
	m_flStartAutomatically = FALSE;
	m_nLogRotation = 0;
	m_nTimestampFormat = -1;
	m_flLogEmail = FALSE;
	//}}AFX_DATA_INIT
	}

CGeneralPage::~CGeneralPage()
	{
	}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
	{
	//TRACE0("CGeneralPage::DoDataExchange\n");
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	DDX_Control(pDX, IDC_LOGFILENAMEBTN, m_ctlLogFilenameBtn);
	DDX_Control(pDX, IDC_LOGFILENAME, m_ctlLogFilename);
	DDX_Control(pDX, IDC_PORTNAME, m_ctlPortName);
	DDX_Control(pDX, IDC_LOGROTATIONFILE, m_ctlLogRotationFile);
	DDX_CBString(pDX, IDC_PORTNAME, m_strPortName);
	DDX_CBString(pDX, IDC_BAUDRATE, m_strBaudRate);
	DDX_Text(pDX, IDC_LOGFILENAME, m_strLogFilename);
	DDX_CBIndex(pDX, IDC_BYTESIZE, m_nByteSize);
	DDX_CBIndex(pDX, IDC_PARITY, m_nParity);
	DDX_CBIndex(pDX, IDC_STOPBITS, m_nStopBits);
	DDX_Check(pDX, IDC_LOGCONFIG, m_flLogConfig);
	DDX_Check(pDX, IDC_LOGSTARTSTOP, m_flLogStartStop);
	DDX_Check(pDX, IDC_LOGTIMEOUTS, m_flLogTimeouts);
	DDX_Check(pDX, IDC_STARTAUTOMATICALLY, m_flStartAutomatically);
	DDX_CBIndex(pDX, IDC_LOGROTATION, m_nLogRotation);
	DDX_CBIndex(pDX, IDC_TIMESTAMPFORMAT, m_nTimestampFormat);
	DDX_Check(pDX, IDC_LOGEMAIL, m_flLogEmail);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_BN_CLICKED(IDC_LOGFILENAMEBTN, OnLogFilenameBtn)
	ON_BN_CLICKED(IDC_LOGOPEN, OnLogOpen)
	ON_EN_CHANGE(IDC_LOGFILENAME, OnChangeLogFilename)
	ON_CBN_SELCHANGE(IDC_LOGROTATION, OnSelChangeLogRotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

BOOL CGeneralPage::OnInitDialog() 
	{
	CPropertyPage::OnInitDialog();
	
	//m_pMainWnd = (CMainWnd*) GetOwner()->GetOwner();
	//m_pMainWnd = (CMainWnd*) GetOwner();
	//ASSERT(m_pMainWnd);
	//CString s;
	//pMainWnd->GetWindowText(s);
	//TRACE1("CGeneralPage::OnInitDialog: pMainWnd=%s\n", s);
	//m_pMainWnd->UpdateStatus();

	UpdateLogRotationFileFormat();

	m_ctlPortName.EnableWindow(!flMultipleSelection);
	m_ctlLogFilename.EnableWindow(!flMultipleSelection);
	m_ctlLogFilenameBtn.EnableWindow(!flMultipleSelection);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CGeneralPage::OnLogFilenameBtn() 
	{
	CString strInitialFilename;
	GetDlgItemText(IDC_LOGFILENAME, strInitialFilename);

	char szFilter[] = "All Files (*.*)|*.*||";

	CFileDialog fd(FALSE, NULL, strInitialFilename, OFN_HIDEREADONLY, szFilter);
	int iRc = fd.DoModal();
	if (iRc == IDCANCEL && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
		{
		fd.m_ofn.lpstrFile[0] = 0;
		iRc = fd.DoModal();
		}
	if (iRc == IDOK)
		SetDlgItemText(IDC_LOGFILENAME, fd.GetPathName());
	}

void CGeneralPage::OnLogOpen() 
	{
	CString strFilename;
	string s;

	UpdateData(TRUE);

	CInstanceParameters ipTemp = *ip;

	ipTemp.strLogFilename = m_strLogFilename;
	ipTemp.dwLogRotation = m_nLogRotation;
	ipTemp.ParseLogFilename();

	LONGLONG llRotateRangeBegin, llRotateRangeEnd;
	CalcCurrentLogFileRotation(ipTemp, s, llRotateRangeBegin, llRotateRangeEnd);

	strFilename = s.c_str();

	TRACE1("CGeneralPage::OnLogOpen: filename=%s\n", strFilename);

	HINSTANCE rc = ShellExecute(m_hWnd, NULL, strFilename, NULL, NULL, SW_SHOWNORMAL);
	TRACE1("CGeneralPage::OnLogOpen: ShellExecute=%d\n", rc);
	if (rc == (HINSTANCE) SE_ERR_NOASSOC)
		{
		rc = ShellExecute(m_hWnd, NULL, "NOTEPAD.EXE", strFilename, NULL, SW_SHOWNORMAL);
		TRACE1("CGeneralPage::OnLogOpen: ShellExecute(2)=%d\n", rc);
		}
	if ((unsigned) rc <= 32)
		{
		CString s;
		s.Format("Unable to open the log file \'%s\'.  Make sure the log file "
			"exists and that it is associated with an application.  "
			"(Error %u)", strFilename, rc);
		((CSMConfigApp*) AfxGetApp())->DisplayError(s);
		}
	}

void CGeneralPage::OnChangeLogFilename() 
	{
	//TRACE0("CGeneralPage::OnChangeLogFilename\n");
	UpdateLogRotationFileFormat();
	}

void CGeneralPage::OnSelChangeLogRotation() 
	{
	//TRACE0("CGeneralPage::OnEditChangeLogRotation\n");
	UpdateLogRotationFileFormat();
	}

void CGeneralPage::UpdateLogRotationFileFormat()
	{
	UpdateData(TRUE);
	//TRACE2("CMainWnd::UpdateLogRotationFileFormat: file=%s, log rotation=%d",
	//	m_pGeneralPage->m_strLogFilename, m_pGeneralPage->m_nLogRotation);

	// Create temporary InstanceParameters object to calculate
	// the log filename.  Don't change the actual IP because it shouldn't be
	// updated until the user presses OK.
	CInstanceParameters ipTemp = *ip;
	
	ipTemp.strLogFilename = m_strLogFilename;
	ipTemp.dwLogRotation = m_nLogRotation;
	ipTemp.ParseLogFilename();

	CString prefix, file, seq, s;
	prefix = ipTemp.strLogFilenamePrefix.c_str();
	file = GetFilenameFromPath(prefix);

	s = file;
	switch (ipTemp.dwLogRotation)
		{
		case LOGROTATION_NONE:		seq = ""; break;
		case LOGROTATION_MONTHLY:	seq = "yyyymm"; break;
		case LOGROTATION_WEEKLY:	seq = "yyyymmdd"; break;
		case LOGROTATION_DAILY:		seq = "yyyymmdd"; break;
		case LOGROTATION_HOURLY:	seq = "yyyymmddhh"; break;
		case LOGROTATION_MINUTELY:	
		default:					seq = "yyyymmddhhmm"; break;
		}
	s += seq;
	s += ipTemp.strLogFilenameSuffix.c_str();

	m_ctlLogRotationFile.SetWindowText(s);	
	}
