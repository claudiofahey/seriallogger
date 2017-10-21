/*****************************************************************************
 SMConfig.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "SMConfig.h"

#include "MainFrm.h"
#include "InstanceListDoc.h"

#include "InstanceListView.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMConfigApp

BEGIN_MESSAGE_MAP(CSMConfigApp, CWinApp)
	//{{AFX_MSG_MAP(CSMConfigApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMConfigApp construction

CSMConfigApp::CSMConfigApp()
	{
	}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSMConfigApp object

CSMConfigApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSMConfigApp initialization

BOOL CSMConfigApp::InitInstance()
	{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_strHelpFilename = GetAppDirectory() + "\\" SZAPPNAME ".chm";

	// Initialize reg code seed
	SetRegCodeSeed(REGCODESEED);

	if (m_lpCmdLine[0] != 0)
		{
		TRACE1("CSMConfigApp::InitInstance: cmdline=%s\n", m_lpCmdLine);
		// "/removeall" is used by 1.2 and above.  However, if upgraded from
		// less than 1.2, then uninstall will run "/removeservice".
		if (stricmp(m_lpCmdLine, "/removeall") == 0 ||
			stricmp(m_lpCmdLine, "/removeservice") == 0)
			{
			TRACE0("CSMConfigApp::InitInstance: removing all services\n");
			RemoveAll();
			return FALSE;
			}
		}

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInstanceListDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CInstanceListView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;	
	}

int CSMConfigApp::DisplayError(LPCTSTR lpszText)
	{
	return m_pMainWnd->MessageBox(lpszText, NULL, MB_ICONERROR | MB_OK);
	//return ::MessageBox(NULL, lpszText, SZCONFIGAPPNAME " Error", MB_ICONERROR | MB_OK);
	}

BOOL CSMConfigApp::RemoveAll()
	{
	BOOL flSuccess = TRUE;
	LONG lRc = 0;
    SC_HANDLE schSCManager = 0;

	TRACE("CSMConfigApp::RemoveAll:\n");

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CInstanceListDoc);
	CInstanceListDoc *pDoc = (CInstanceListDoc*) pRuntimeClass->CreateObject();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CInstanceListDoc)));
	
	//pDoc->DisplayError("Remove All");

	std::map<unsigned, CEnumServiceStatus> mapEss;
	std::map<unsigned, CEnumServiceStatus>::const_iterator iEss;

	if (!pDoc->EnumServices(mapEss))
		flSuccess = FALSE;
	
	//
	// Create service instances if necessary, set service auto start, and set parameters
	//

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		CString s;
		s.Format("OpenSCManager failed: %s", GetLastErrorText());
		DisplayError(s);
		flSuccess = FALSE;
		}

	for (iEss = mapEss.begin() ; iEss != mapEss.end() ; iEss++)
		{
		TRACE("CSMConfigApp::RemoveAll: Ess instance=%u\n", iEss->first);
		if (pDoc->RemoveService(iEss->second, schSCManager))
			flSuccess = FALSE;
		}

	CloseServiceHandle(schSCManager);
	schSCManager = 0;

	//
	// Delete common registry keys
	//

	string strKeyNameCommon = SZREGKEYCOMMON;
	lRc = RegDeleteKey(HKEY_LOCAL_MACHINE, strKeyNameCommon.c_str());
	if (lRc != ERROR_SUCCESS)
		{
		if (lRc != ERROR_FILE_NOT_FOUND)
			{
			CString s;
			s.Format("RegDeleteKey failed on \"%s\": %s", strKeyNameCommon.c_str(), GetErrorText(lRc));
			DisplayError(s);
			flSuccess = FALSE;
			}
		}

	string strKeyNameCompany = SZREGKEYCOMPANY;
	lRc = RegDeleteKeyIfEmpty(HKEY_LOCAL_MACHINE, strKeyNameCompany.c_str());
	if (lRc != ERROR_SUCCESS)
		{
		if (lRc != ERROR_FILE_NOT_FOUND)
			{
			CString s;
			s.Format("RegDeleteKey failed on \"%s\": %s", strKeyNameCompany.c_str(), GetErrorText(lRc));
			DisplayError(s);
			flSuccess = FALSE;
			}
		}

	delete pDoc;
	pDoc = 0;

	return flSuccess;
	}

void CSMConfigApp::WinHelp(DWORD dwData, UINT nCmd) 
	{
	BOOL flOpenContents = TRUE;

	TRACE("CSMConfigApp::WinHelp: nCmd=%u, dwData=0x%x\n", nCmd, dwData);
	TRACE("CSMConfigApp::WinHelp: help file=%s\n", m_strHelpFilename);

	HWND hwndHelp = 0;

	switch (nCmd)
		{
		case HELP_CONTEXT:
			hwndHelp = HtmlHelp(NULL, m_strHelpFilename, HH_HELP_CONTEXT, dwData);
			//TRACE("CSMConfigApp::WinHelp: hwnd=%p\n", hwndHelp);
			if (hwndHelp)
				flOpenContents = FALSE;
			else
				{
				TRACE("CSMConfigApp::WinHelp: context help failed; attempting to open contents...\n");
				}
			break;
		}

	if (flOpenContents)
		{
		hwndHelp = HtmlHelp(NULL, m_strHelpFilename, HH_DISPLAY_TOPIC, 0);
		//TRACE("CSMConfigApp::WinHelp: hwnd=%p\n", hwndHelp);
		if (hwndHelp == 0)
			{
			typedef struct tagHH_LAST_ERROR
				{
				int cbStruct;
				HRESULT hr;
				CComBSTR description;
				} HH_LAST_ERROR;
			HH_LAST_ERROR hhErr;
			ZeroMemory(&hhErr, sizeof(HH_LAST_ERROR));
			hhErr.cbStruct = sizeof(HH_LAST_ERROR);
			hwndHelp = HtmlHelp(NULL, NULL, HH_GET_LAST_ERROR, (DWORD) &hhErr);
			// Make sure that HH_GET_LAST_ERROR succeeded.
			if (hwndHelp)
				{
				// Only report an error if we found one
				if (FAILED(hhErr.hr))
					{
					// Is there a text message to display...
					if (hhErr.description)
						{
						USES_CONVERSION;
						// Convert the String to ANSI
						TCHAR* pDesc = OLE2T(hhErr.description) ;
						// Display
						TRACE("CSMConfigApp:WinHelp: err=%s\n", pDesc);
						AfxMessageBox(pDesc, MB_ICONERROR | MB_OK);
						}
					}
				}			
			}
		}	
	}

void CSMConfigApp::SetStatusText(LPCTSTR lpszText)
	{
	if (m_pMainWnd && ((CMainFrame*) m_pMainWnd)->m_wndStatusBar)
		{
		ASSERT(m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
		((CMainFrame*) m_pMainWnd)->m_wndStatusBar.SetWindowText(lpszText);
		}
	else
		{
		TRACE("CSMConfigApp::SetStatusText: status bar not available for message \"%s\"\n",
			lpszText);
		}
	}
