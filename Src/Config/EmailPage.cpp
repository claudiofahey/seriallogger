/*****************************************************************************
 EmailPage.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "SMConfig.h"
#include "GeneralPage.h"
#include "EmailPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CConfigAppSmtp: public CSmtp
	{
	public:
	CEmailPage *pEmailPage;
	virtual int SmtpError(int nError, LPTSTR pszErr);
	};

/////////////////////////////////////////////////////////////////////////////
// CEmailPage property page

IMPLEMENT_DYNCREATE(CEmailPage, CPropertyPage)

CEmailPage::CEmailPage():
	CPropertyPage(CEmailPage::IDD),
	ip(0),
	pGeneralPage(0)
	{
	//{{AFX_DATA_INIT(CEmailPage)
	m_strSmtpFromAddress = _T("");
	m_strSmtpServer = _T("");
	m_strSmtpToAddress = _T("");
	//}}AFX_DATA_INIT
	}

CEmailPage::~CEmailPage()
	{
	}

void CEmailPage::DoDataExchange(CDataExchange* pDX)
	{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmailPage)
	DDX_Text(pDX, IDC_SMTPFROMADDRESS, m_strSmtpFromAddress);
	DDX_Text(pDX, IDC_SMTPSERVER, m_strSmtpServer);
	DDX_Text(pDX, IDC_SMTPTOADDRESS, m_strSmtpToAddress);
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CEmailPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEmailPage)
	ON_BN_CLICKED(IDC_SENDNOW, OnSendNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmailPage message handlers

void CEmailPage::OnSendNow() 
	{
	ASSERT(ip);
	ASSERT(pGeneralPage);

	UpdateData(TRUE);
	pGeneralPage->UpdateData(TRUE);

	CInstanceParameters ipTemp = *ip;
	ipTemp.strLogFilename = pGeneralPage->m_strLogFilename;
	ipTemp.dwLogRotation = pGeneralPage->m_nLogRotation;
	ipTemp.ParseLogFilename();
	ipTemp.strSmtpServer = m_strSmtpServer;
	ipTemp.strSmtpFromAddress = m_strSmtpFromAddress;
	ipTemp.strSmtpToAddress = m_strSmtpToAddress;
	ipTemp.CalcParameters();	// parse e-mail addresses

	try
		{
		if (ipTemp.strSmtpServer.empty())
			throw string("You must enter the SMTP server address.");

		if (ipTemp.strSmtpFromAddress.empty())
			throw string("You must enter the sender address.");
		
		if (ipTemp.listSmtpToAddress.size() == 0)
			throw string("You must enter at least one recipient.");
		}
	catch (string s)
		{
		MessageBox(s.c_str(), "Error", MB_ICONERROR);
		return;
		}

	LONGLONG llRotateRangeBegin, llRotateRangeEnd;
	string strLogFilename;
	CalcCurrentLogFileRotation(ipTemp, strLogFilename, llRotateRangeBegin, llRotateRangeEnd);
	TRACE("CEmailPage::OnSendNow: logfile=%s\n", strLogFilename.c_str());

	int iRc = MessageBox((string("Are you sure you want to send the current log file \"") +
		strLogFilename + "\"?  "
		"If the log file is large, this may take a long time.").c_str(), "E-mail Current Log", 
		MB_ICONQUESTION | MB_YESNOCANCEL);
	if (iRc != IDYES)
		return;

	AfxGetApp()->BeginWaitCursor();
	((CSMConfigApp*)AfxGetApp())->SetStatusText("Sending e-mail...");

	CMailThreadInfo mti;
	mti.strLogFilename = strLogFilename;
	PrepareMailThreadInfo(mti, ipTemp);

	TRACE("CEmailPage::OnSendNow: smtp=%s\n", mti.strSmtpServer.c_str());

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);

	CConfigAppSmtp mail;
	mail.pEmailPage = this;

	if (mail.Connect((char*) mti.strSmtpServer.c_str()))
		{
		int iRc = mail.SendMessage(mti.msg);
		mail.Close();
		((CSMConfigApp*)AfxGetApp())->SetStatusText("Sending e-mail...Done.");
		if (iRc)
			{
			stringstream ss;
			ss << "Unable to send SMTP message.  Error code is " << iRc << ".";
			MessageBox(ss.str().c_str(), "SMTP Error", MB_ICONERROR);
			}
		else
			{
			string s = string("Successfully e-mailed log file ") + mti.strLogFilename;
			MessageBox(s.c_str(), "E-mail Successful", MB_ICONINFORMATION);
			}
		}
	else
		{
		((CSMConfigApp*)AfxGetApp())->SetStatusText("Sending e-mail...Done.");
		MessageBox("Unable to connect to SMTP server.", "SMTP Connection Error", MB_ICONERROR);
		}

	AfxGetApp()->EndWaitCursor();
	}

int CConfigAppSmtp::SmtpError(int nError, LPTSTR pszErr)
	{
	stringstream ss;
	ss << "SMTP error " << nError << ": " << pszErr;
	pEmailPage->MessageBox(ss.str().c_str(), "SMTP Error", MB_ICONERROR);
	return 0;
	}
