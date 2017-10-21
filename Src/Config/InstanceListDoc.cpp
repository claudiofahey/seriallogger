/*****************************************************************************
 InstanceListDoc.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "smconfig.h"
#include "InstanceListDoc.h"
#include "AboutDlg.h"
#include "RegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc

IMPLEMENT_DYNCREATE(CInstanceListDoc, CDocument)

CInstanceListDoc::CInstanceListDoc()
	{
	}

CInstanceListDoc::~CInstanceListDoc()
	{
	}

BEGIN_MESSAGE_MAP(CInstanceListDoc, CDocument)
	//{{AFX_MSG_MAP(CInstanceListDoc)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_COMMAND(ID_EVENTVIEWER, OnEventViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc diagnostics

#ifdef _DEBUG
void CInstanceListDoc::AssertValid() const
	{
	CDocument::AssertValid();
	}

void CInstanceListDoc::Dump(CDumpContext& dc) const
	{
	CDocument::Dump(dc);
	}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc serialization

void CInstanceListDoc::Serialize(CArchive& ar)
	{
	if (ar.IsStoring())
		{
		// TODO: add storing code here
		}
	else
		{
		// TODO: add loading code here
		}
	}

/////////////////////////////////////////////////////////////////////////////
// CInstanceListDoc commands

BOOL CInstanceListDoc::OnNewDocument()
	{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//
	// Load common parameters
	//
	if (!cp.LoadParameters())
		cp.LoadDownlevelParameters();
	cp.CalcParameters();

	//
	// Enumerate services and load instance parameters
	//
	std::map<unsigned, CEnumServiceStatus> mapEss;
	std::map<unsigned, CEnumServiceStatus>::const_iterator iEss;

	if (!EnumServices(mapEss))
		return FALSE;

	//
	// Load instance parameters
	//
	for (iEss = mapEss.begin() ; iEss != mapEss.end() ; iEss++)
		{
		TRACE("CInstanceListDoc::OnNewDocument: Ess instance=%u\n", iEss->first);

		CInstanceParameters ip;
		ip.strServiceName = iEss->second.strServiceName;
		ip.strServiceDisplayName = iEss->second.strDisplayName;

		if (!ip.LoadParameters())
			{
			TRACE("CInstanceListDoc::OnNewDocument: unable to load instance parameters\n");
			if (ip.listPp.size() < 1)
				{
				CPortParameters pp;
				ip.listPp.push_back(pp);
				}
			}
		mapIp[iEss->first] = ip;
		}

	//
	// Determine if a service with name "SerialLogger" exists (instance = 0).
	// If so, then move parameters to instance 1.
	// Upon save, the old service will be removed and new service will be created.
	//
	if (mapIp.find(0) != mapIp.end())
		{
		TRACE("CInstanceListDoc::OnNewDocument: instance 0 found\n");
		if (mapIp.find(1) == mapIp.end())
			{
			TRACE("CInstanceListDoc::OnNewDocument: transfering instance 0 to 1\n");
			mapIp[1] = mapIp[0];
			mapIp[1].SetServiceInstance(1);
			}
		mapIp.erase(0);
		SetModifiedFlag(TRUE);
		}

	//
	// If no instances exist, then create a first instance
	//
	if (mapIp.size() <= 0)
		{
		TRACE("CInstanceListDoc::OnNewDocument: no instances found, creating first instance\n");
		CInstanceParameters ip;
		CPortParameters pp;
		ip.listPp.push_back(pp);
		ip.SetServiceInstance(1);
		mapIp[1] = ip;
		SetDefaults(1);
		SetModifiedFlag(TRUE);
		}

	GetServiceConfig();

	return TRUE;
	}

void CInstanceListDoc::SetDefaults(unsigned nInstance)
	{	
	// Find next available COM port
	std::map<unsigned, CInstanceParameters>::const_iterator iIp;
	stringstream ssPortName;
	for (unsigned nPort = 1 ; ; nPort++)
		{
		ssPortName.str("");
		ssPortName << "COM" << nPort;
		for (iIp = mapIp.begin() ; iIp != mapIp.end() ; iIp++)
			{
			if (iIp->second.listPp[0].strPortName == ssPortName.str())
				break;
			}
		if (iIp == mapIp.end())
			{
			mapIp[nInstance].listPp[0].strPortName = ssPortName.str();
			break;
			}
		}

	// set log file
	stringstream ssLogFilename;
	ssLogFilename << (const char*) GetAppDirectory() << "\\" SZAPPNAME << nInstance << ".log";
	mapIp[nInstance].strLogFilename = ssLogFilename.str();
	}

BOOL CInstanceListDoc::EnumServices(std::map<unsigned, CEnumServiceStatus> &mapEss)
	{
	//
	// Connect to SC Manager
	//

    SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	SERVICE_STATUS ssStatus = {0};

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		CString s;
		s.Format("OpenSCManager failed: %s", GetLastErrorText());
		DisplayError(s);
		return FALSE;
		}

	//
	// Enumerate all services
	//

	BOOL flRc = 0;
	const DWORD cbBufSize = 0x10000;		// 64 KB buffer
	DWORD cbBytesNeeded = 0;
	unsigned char pBuf[cbBufSize] = {0};
	DWORD cServicesReturned = 0;
	DWORD ResumeHandle = 0;
	ENUM_SERVICE_STATUS *ess = (ENUM_SERVICE_STATUS*) pBuf;
	DWORD dwErr = 0;

	do
		{
		dwErr = 0;
		flRc = EnumServicesStatus(schSCManager, SERVICE_WIN32, SERVICE_STATE_ALL, ess, cbBufSize,
			&cbBytesNeeded, &cServicesReturned, &ResumeHandle);
		if (!flRc)
			{
			dwErr = GetLastError();
			//TRACE("CInstanceListDoc::Save: EnumServiceStatus err=%u\n", dwErr);
			if (dwErr != ERROR_MORE_DATA)
				{
				CString s;
				s.Format("EnumServicesStatus failed: %s", GetLastErrorText());
				DisplayError(s);

				CloseServiceHandle(schSCManager);
				schSCManager = 0;

				return FALSE;
				}
			}

		for (unsigned i = 0 ; i < cServicesReturned ; i++)
			{
			//TRACE("CInstanceListDoc::Save: EnumServiceStatus(%u): %s\n", i, ess[i].lpServiceName);
			if (strnicmp(ess[i].lpServiceName, SZSERVICENAME, strlen(SZSERVICENAME)) == 0)
				{
				char *endptr = 0;
				unsigned nInstance = strtoul(ess[i].lpServiceName + strlen(SZSERVICENAME), &endptr, 10);

				if (*endptr == 0)	// verify that only a valid instance # (or nothing) follows
					{
					// if nothing follows (service name is "SerialLogger"), then nInstance will be 0

					TRACE("CInstanceListDoc::Save: match instance=%u\n", nInstance);
					CEnumServiceStatus Ess(ess[i].lpServiceName, ess[i].lpServiceName, ess[i].ServiceStatus);
					mapEss[nInstance] = Ess;
					}
				else
					{
					TRACE("CInstanceListDoc::Save: service name has invalid instance #\n");
					}
				}
			}
		}
	while (dwErr == ERROR_MORE_DATA);

	CloseServiceHandle(schSCManager);
	schSCManager = 0;

	return TRUE;
	}

BOOL CInstanceListDoc::Save()
	{
	LONG lRc = 0;
	BOOL flRc = 0;
	DWORD dw = 0;
	BOOL flSuccess = TRUE;
    SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	SERVICE_STATUS ssStatus = {0};
	const DWORD cbBufSize = 0x10000;		// 64 KB buffer
	DWORD cbBytesNeeded = 0;
	unsigned char pBuf[cbBufSize] = {0};
	std::map<unsigned, CEnumServiceStatus> mapEss;
	std::map<unsigned, CEnumServiceStatus>::const_iterator iEss;
	std::map<unsigned, CInstanceParameters>::const_iterator iIp;

	TRACE("CInstanceListDoc::Save\n");

	AfxGetApp()->BeginWaitCursor();

	//
	// Write common registry parameters
	//

	SetStatusText("Updating the registry (phase 1)...");

	HKEY hkeyCommon = 0;
	string strKeyNameCommon = SZREGKEYCOMMON;

	lRc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKeyNameCommon.c_str(), 
		0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkeyCommon, &dw);
	if (lRc != ERROR_SUCCESS)
		{
		CString s;
		s.Format("RegCreateKeyEx failed: %s", GetLastErrorText());
		DisplayError(s);
		hkeyCommon = 0;
		flSuccess = FALSE;
		}
	else
		{
		if (cp.flRegistered)
			{
			flRc = RegSetString(hkeyCommon, "RegName", cp.strRegName);
			flRc = RegSetInt(hkeyCommon, "RegCode1", cp.dwRegCode1);
			}

		RegCloseKey(hkeyCommon);
		hkeyCommon = 0;
		}

	//
	// Enumerate services
	//

	SetStatusText("Creating and updating services...");

	if (!EnumServices(mapEss))
		return FALSE;

	//
	// Create service instances if necessary, set service auto start, and set parameters
	//

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		CString s;
		s.Format("OpenSCManager failed: %s", GetLastErrorText());
		DisplayError(s);
		return FALSE;
		}

	// determine service executable filename
	string strServiceFilename = string(GetAppDirectory()) + "\\" SZAPPNAME ".exe";

	for (iIp = mapIp.begin() ; iIp != mapIp.end() ; iIp++)
		{
		TRACE("CInstanceListDoc::Save: sp instance=%u\n", iIp->first);

		DWORD dwDesiredStartType = iIp->second.flStartAutomatically ? 
			SERVICE_AUTO_START : SERVICE_DEMAND_START;

		BOOL flServiceExists = FALSE;

		// Determine if service instance already exists
		iEss = mapEss.find(iIp->first);
		if (iEss == mapEss.end())
			{
			//
			// Service doesn't exist; Create service instance
			//

			TRACE("CInstanceListDoc::Save: instance %u not found in SCM\n", iIp->first);

			//string strDisplayName = GetServiceDisplayName(iIp->first);

			string strBinaryPathName = string("\"") + strServiceFilename + "\"";
			//TRACE("BinaryPathName=%s\n", ssBinaryPathName.str().c_str());

			schService = CreateService(
				schSCManager,				// SCManager database
				iIp->second.strServiceName.c_str(),			// name of service
				iIp->second.strServiceDisplayName.c_str(),	// name to display
				SERVICE_ALL_ACCESS,         // desired access
				SERVICE_WIN32_OWN_PROCESS,  // service type
				dwDesiredStartType,			// start type
				SERVICE_ERROR_NORMAL,       // error control type
				strBinaryPathName.c_str(),	// service's binary
				NULL,                       // no load ordering group
				NULL,                       // no tag identifier
				TEXT(SZDEPENDENCIES),       // dependencies
				NULL,                       // LocalSystem account
				NULL);                      // no password

			if (schService)
				{
				flServiceExists = TRUE;
				CloseServiceHandle(schService);
				schService = 0;
				}
			else if (GetLastError() != ERROR_SERVICE_EXISTS) 
				{
				CString s;
				s.Format("CreateService failed when trying to create service \"%s\": %s", 
					iIp->second.strServiceName.c_str(), GetLastErrorText());
				CloseServiceHandle(schSCManager);
				DisplayError(s);
				flSuccess = FALSE;
				}
			}
		else
			{
			//
			// Service already exists; Check and set service autostart setting
			//

			flServiceExists = TRUE;

			schService = ::OpenService(schSCManager, iEss->second.strServiceName.c_str(),
				SERVICE_ALL_ACCESS);

			if (!schService)
				{
				CString s;
				s.Format("OpenService failed on service \"%s\": %s", 
					iEss->second.strServiceName.c_str(), GetLastErrorText());
				DisplayError(s);
				flSuccess = FALSE;
				}
			else
				{
				QUERY_SERVICE_CONFIG *qsc = (QUERY_SERVICE_CONFIG*) pBuf;
				flRc = QueryServiceConfig(schService, qsc, cbBufSize, &cbBytesNeeded);
				if (!flRc)
					{
					CString s;
					s.Format("QueryServiceConfig failed on service \"%s\": %s", 
						iEss->second.strServiceName.c_str(), GetLastErrorText());
					DisplayError(s);
					}
				if (!flRc || qsc->dwStartType != dwDesiredStartType)
					{
					TRACE("CInstanceListDoc::Save: changing start type to %u\n", dwDesiredStartType);

					flRc = ChangeServiceConfig(schService, SERVICE_NO_CHANGE,
							dwDesiredStartType,	SERVICE_NO_CHANGE, 0, 0, 0, 0, 0, 0, 0);						
					if (!flRc)
						{
						CString s;
						s.Format("ChangeServiceConfig failed on service \"%s\": %s", 
							iEss->second.strServiceName.c_str(), GetLastErrorText());
						DisplayError(s);
						flSuccess = FALSE;
						}
					}

				CloseServiceHandle(schService);
				schService = 0;
				}
			}

		//
		// If service already exists or was just created, set registry parameters
		//

		if (flServiceExists)
			{
			//
			// Update service parameters registry keys
			//

			string strKeyNameParameters = string("SYSTEM\\CurrentControlSet\\Services\\") + 
				iIp->second.strServiceName + "\\Parameters";

			HKEY hkeyInstance = 0;
			lRc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKeyNameParameters.c_str(), 
				0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkeyInstance, &dw);
			if (lRc != ERROR_SUCCESS)
				{
				hkeyInstance = 0;
				flSuccess = FALSE;
				}
			else
				{
				const CInstanceParameters &ip = iIp->second;
				flRc = RegSetString(hkeyInstance, "LogFilename", ip.strLogFilename);
				flRc = RegSetInt(hkeyInstance, "TimestampFormat", ip.dwTimestampFormat);
				flRc = RegSetInt(hkeyInstance, "LogRotation", ip.dwLogRotation);
				flRc = RegSetString(hkeyInstance, "SmtpFromAddress", ip.strSmtpFromAddress);
				flRc = RegSetString(hkeyInstance, "SmtpToAddress", ip.strSmtpToAddress);
				flRc = RegSetString(hkeyInstance, "SmtpServer", ip.strSmtpServer);
				flRc = RegSetInt(hkeyInstance, "PauseTimeout", ip.dwPauseTimeout);
				flRc = RegSetInt(hkeyInstance, "VerboseLevel", ip.dwVerboseLevel);
				flRc = RegSetString(hkeyInstance, "SQLConnectionString", ip.strSQLConnectionString);
				flRc = RegSetString(hkeyInstance, "SQLWriteLineStatement", ip.strSQLWriteLineStatement);
				flRc = RegSetInt(hkeyInstance, "MaxLineLength", ip.dwMaxProcessedLineLength);

				HKEY hkeyPort = 0;

				lRc = RegCreateKeyEx(hkeyInstance, "Port1", 
					0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkeyPort, &dw);
				if (lRc != ERROR_SUCCESS)
					{
					hkeyPort = 0;
					flSuccess = FALSE;
					}
				else
					{
					const CPortParameters &pp = iIp->second.listPp[0];
					flRc = RegSetString(hkeyPort, "PortName", pp.strPortName);
					flRc = RegSetInt(hkeyPort, "BaudRate", pp.BaudRate);
					flRc = RegSetInt(hkeyPort, "ByteSize", pp.ByteSize);
					flRc = RegSetInt(hkeyPort, "Parity", pp.Parity);
					flRc = RegSetInt(hkeyPort, "StopBits", pp.StopBits);
					flRc = RegSetInt(hkeyPort, "ComBufferSize", pp.dwComBufferSize);
					flRc = RegSetInt(hkeyPort, "Timeout", pp.dwTimeout);
					flRc = RegSetInt(hkeyPort, "WarningInterval", pp.dwWarningInterval);
					flRc = RegSetInt(hkeyPort, "DayOfWeekMask", pp.dwDayOfWeekMask);
					flRc = RegSetInt(hkeyPort, "StartHour", pp.dwStartHour);
					flRc = RegSetInt(hkeyPort, "StartMin", pp.dwStartMin);
					flRc = RegSetInt(hkeyPort, "EndHour", pp.dwEndHour);
					flRc = RegSetInt(hkeyPort, "EndMin", pp.dwEndMin);
					flRc = RegSetString(hkeyPort, "NetSendUsers", pp.strNetSendUsers);
					flRc = RegSetString(hkeyPort, "CustomTimeoutCmd", pp.strTimeoutCmd);
					//flRc = RegSetInt(hkeyPort, "VerboseLevel", pp.dwVerboseLevel);

					RegCloseKey(hkeyPort);
					hkeyPort = 0;
					}

				RegCloseKey(hkeyInstance);
				hkeyInstance = 0;
				}			

			//
			// Update event logging registry keys
			//

			HKEY hkeyEventLog = 0;
			string strKeyNameEventLog = string("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\") +
				iIp->second.strServiceName;

			lRc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKeyNameEventLog.c_str(), 
				0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkeyEventLog, &dw);
			if (lRc != ERROR_SUCCESS)
				{
				CString s;
				s.Format("RegCreateKeyEx failed: %s", GetLastErrorText());
				DisplayError(s);
				flSuccess = FALSE;
				}
			else
				{
				lRc = RegSetString(hkeyEventLog, "EventMessageFile", strServiceFilename);
				lRc = RegSetInt(hkeyEventLog, "TypesSupported", EVENTLOG_ERROR_TYPE | 
					EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE);

				RegCloseKey(hkeyEventLog);
				hkeyEventLog = 0;
				}
			}
		}

	//
	// Stop and delete unused services; removing a service automatically deletes its parameters
	//

	for (iEss = mapEss.begin() ; iEss != mapEss.end() ; iEss++)
		{
		TRACE("CInstanceListDoc::Save: Ess instance=%u\n", iEss->first);
		if (mapIp.find(iEss->first) == mapIp.end())
			{
			TRACE("CInstanceListDoc::Save: extra instance %u found in SCM\n", iEss->first);

			SetStatusText((string("Stopping and deleting ") + 
				iEss->second.strServiceName + "...").c_str());

			if (!RemoveService(iEss->second, schSCManager))
				flSuccess = FALSE;
			}
		}

	CloseServiceHandle(schSCManager);
	schSCManager = 0;

	if (flSuccess)
		{
		SetModifiedFlag(FALSE);
		}

	SetStatusText(flSuccess ? "Configuration saved." : "Configuration NOT saved.");

	GetServiceStatus();
	//UpdateAllViews(NULL);

	AfxGetApp()->EndWaitCursor();

	return flSuccess;
	}

BOOL CInstanceListDoc::RemoveService(const CEnumServiceStatus &Ess, SC_HANDLE schSCManager)
	{
	LONG lRc = 0;
	//BOOL flRc = 0;
	//DWORD dw = 0;
	BOOL flSuccess = TRUE;
    //SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	SERVICE_STATUS ssStatus = {0};

	schService = ::OpenService(schSCManager, Ess.strServiceName.c_str(), SERVICE_ALL_ACCESS);

	if (!schService)
		{
		if (GetLastError() != ERROR_SERVICE_DOES_NOT_EXIST)
			{
			CString s;
			s.Format("OpenService failed on service \"%s\": %s", 
				Ess.strServiceName.c_str(), GetLastErrorText());
			DisplayError(s);
			flSuccess = FALSE;
			}
		}
	else
		{
		// try to stop the service if it is running and stopable
		if (Ess.ServiceStatus.dwCurrentState != SERVICE_STOPPED && 
			(Ess.ServiceStatus.dwControlsAccepted & SERVICE_ACCEPT_STOP))
			{
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus))
				{
				Sleep(200);

				while(QueryServiceStatus(schService, &ssStatus))
					{
					if (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
						Sleep(200);
					else
						break;
					}
				}
			else if (GetLastError() != ERROR_SERVICE_NOT_ACTIVE)
				{
				CString s;
				s.Format("ControlService failed when trying to stop service \"%s\": %s",
					Ess.strServiceName.c_str(), GetLastErrorText());
				DisplayError(s);
				}
			}

		// now remove the service
		if(!DeleteService(schService))
			{
			if (GetLastError() != ERROR_SERVICE_MARKED_FOR_DELETE)
				{
				CString s;				
				s.Format("DeleteService failed when trying to delete service \"%s\": %s", 
					Ess.strServiceName.c_str(), GetLastErrorText());
				DisplayError(s);
				flSuccess = FALSE;
				}
			}
		else
			{
			TRACE("CInstanceListDoc::RemoveService: deleted service %s\n", Ess.strServiceName.c_str());
			}

		CloseServiceHandle(schService);
		schService = 0;
		}

	//
	// Delete event log registry key
	//

	string strKeyNameEventLog = string("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\") +
		Ess.strServiceName;

	lRc = RegDeleteKey(HKEY_LOCAL_MACHINE, strKeyNameEventLog.c_str());
	if (lRc != ERROR_SUCCESS)
		{
		if (lRc != ERROR_FILE_NOT_FOUND)
			{
			CString s;
			s.Format("RegDeleteKey failed: %s", GetErrorText(lRc));
			DisplayError(s);
			flSuccess = FALSE;
			}
		}

	return flSuccess;
	}

// Gets service auto start status
void CInstanceListDoc::GetServiceConfig()
	{
	BOOL flRc = 0;
    SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	std::map<unsigned, CInstanceParameters>::iterator iIp;
	
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		CString s;
		s.Format("OpenSCManager failed: %s", GetLastErrorText());
		DisplayError(s);
		}
	else
		{	
		const DWORD cbBufSize = 0x10000;		// 64 KB buffer
		DWORD cbBytesNeeded = 0;
		unsigned char pBuf[cbBufSize] = {0};
		QUERY_SERVICE_CONFIG *qsc = (QUERY_SERVICE_CONFIG*) pBuf;

		for (iIp = mapIp.begin() ; iIp != mapIp.end() ; iIp++)
			{
			schService = ::OpenService(schSCManager, iIp->second.strServiceName.c_str(),
				SERVICE_ALL_ACCESS);

			if (!schService)
				{
				if (GetLastError() != ERROR_SERVICE_DOES_NOT_EXIST)
					{
					CString s;
					s.Format("OpenService failed on service \"%s\": %s", 
						iIp->second.strServiceName.c_str(), GetLastErrorText());
					DisplayError(s);
					}
				}
			else
				{
				flRc = QueryServiceConfig(schService, qsc, cbBufSize, &cbBytesNeeded);
				if (!flRc)
					{
					CString s;
					s.Format("QueryServiceConfig failed on service \"%s\": %s", 
						iIp->second.strServiceName.c_str(), GetLastErrorText());
					DisplayError(s);
					}
				else
					{
					iIp->second.flStartAutomatically = (qsc->dwStartType == SERVICE_AUTO_START);
					TRACE("CInstanceListDoc::GetServiceConfig: start auto(%u)=%d\n", 
						iIp->first, iIp->second.flStartAutomatically);
					}
				CloseServiceHandle(schService);
				schService = 0;
				}
			}
		CloseServiceHandle(schSCManager);
		schSCManager = 0;
		}	
	}

// Get service start/stop status
void CInstanceListDoc::GetServiceStatus()
	{
	BOOL flRc = 0;
	BOOL flUpdated = 0;
    SC_HANDLE schSCManager = 0;
    SC_HANDLE schService = 0;
	std::map<unsigned, CInstanceParameters>::iterator iIp;

	//TRACE("CInstanceListDoc::GetServiceStatus: BEGIN\n");
	
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager)
		{
		TRACE("CInstanceListDoc::GetServiceStatus: OpenSCManager failed: %s\n", GetLastErrorText());
		}

	for (iIp = mapIp.begin() ; iIp != mapIp.end() ; iIp++)
		{
		SERVICE_STATUS ssStatus = {0};
		if (schSCManager)
			{
			//
			// Open service and query its status
			//
			//string strServiceName = GetServiceName(iIp->first);
			schService = ::OpenService(schSCManager, iIp->second.strServiceName.c_str(),
				SERVICE_ALL_ACCESS);
			if (schService)
				{
				if (!QueryServiceStatus(schService, &ssStatus))
					{
					TRACE("CInstanceListDoc::GetServiceStatus: QueryServiceStatus failed: %s\n", GetLastErrorText());
					ssStatus.dwCurrentState = 0;
					}
				else
					{
					//
					// If a service is stopped and restart flag is set, then start the service.
					//
					if (iIp->second.flRestart && ssStatus.dwCurrentState == SERVICE_STOPPED)
						{						
						iIp->second.flRestart = FALSE;		// reset flag so we don't restart again
						TRACE("CInstanceListDoc::GetServiceStatus: restarting service %s\n",
							iIp->second.strServiceName.c_str());

						if (!::StartService(schService, 0, NULL))
							{
							if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING)
								{
								CString s;				
								s.Format("StartService failed on service \"%s\": %s", 
									iIp->second.strServiceName.c_str(), GetLastErrorText());
								DisplayError(s);
								}
							}
						else
							iIp->second.flJustStarted = TRUE;
						}
					}

				CloseServiceHandle(schService);
				schService = 0;
				}
			else
				{
				if (GetLastError() != ERROR_SERVICE_DOES_NOT_EXIST)
					{
					TRACE("CInstanceListDoc::GetServiceStatus: OpenService failed on service \"%s\": %s\n", 
						iIp->second.strServiceName.c_str(), GetLastErrorText());
					}
				}
			}

		//
		// If the status has changed, then set the update flag so the view can be udpated.
		//
		if (iIp->second.ssStatus.dwCurrentState != ssStatus.dwCurrentState)
			{
			//iIp->second.flUpdated = TRUE;	// not used
			flUpdated = TRUE;
			}

		//
		// If a service specific error is reported and we just started the service, then
		// display the error.
		//
		if (iIp->second.flJustStarted && ssStatus.dwWin32ExitCode == ERROR_SERVICE_SPECIFIC_ERROR)
			{
			iIp->second.flJustStarted = FALSE;		// reset flag so we don't show the same error again
													// this flag is set in ControlServices()
			CString s;
			CString strServiceSpecificErr;
			// try to load service specific error message from string table
			if (strServiceSpecificErr.LoadString(ssStatus.dwServiceSpecificExitCode))
				s.Format("Service instance %u (%s) failed to start.  "
					"The service-specific error code is %u.\n\n%s\n\n"
					"Please refer to the Event Viewer for details.", 
					iIp->first, iIp->second.listPp[0].strPortName.c_str(), 
					ssStatus.dwServiceSpecificExitCode, strServiceSpecificErr);
			else
				s.Format("Service instance %u (%s) failed to start.  "
					"The service-specific error code is %u.\n\n" 
					"Please refer to the Event Viewer for details.", 
					iIp->first, iIp->second.listPp[0].strPortName.c_str(), 
					ssStatus.dwServiceSpecificExitCode);
			DisplayError(s);
			}

		//
		// Update the status in CInstanceParameters
		//
		memcpy(&iIp->second.ssStatus, &ssStatus, sizeof(SERVICE_STATUS));
		}

	if (schSCManager)
		{
		CloseServiceHandle(schSCManager);
		schSCManager = 0;
		}

	if (flUpdated)
		UpdateAllViews(NULL);

	//TRACE("CInstanceListDoc::GetServiceStatus: END\n");	
	}

BOOL CInstanceListDoc::SaveModified()
	{
	TRACE("CInstanceListDoc::SaveModified\n");

	if (IsModified())
		{
		int iRc = AfxGetApp()->GetMainWnd()->MessageBox(
			"The settings have changed.\n\nDo you want to save the changes?",
			SZCONFIGAPPNAME, MB_ICONEXCLAMATION | MB_YESNOCANCEL);
		switch (iRc)
			{
			case IDYES:
				return Save();
			case IDCANCEL:
				return FALSE;
			default:	// IDNO
				return TRUE;
			}
		}
	else
		return TRUE;
	}

BOOL CInstanceListDoc::RegSetString(HKEY hkey, LPCTSTR szValueName, const string &s)
	{
	BOOL flRc = ::RegSetString(hkey, szValueName, s);
	if (!flRc)
		{
		CString s;				
		s.Format("Unable to set registry key value \"%s\": %s", 
			szValueName, GetLastErrorText());
		DisplayError(s);
		}
	return flRc;
	}

BOOL CInstanceListDoc::RegSetInt(HKEY hkey, LPCTSTR szValueName, DWORD dw)
	{
	BOOL flRc = ::RegSetInt(hkey, szValueName, dw);
	if (!flRc)
		{
		CString s;				
		s.Format("Unable to set registry key value \"%s\": %s", 
			szValueName, GetLastErrorText());
		DisplayError(s);
		}
	return flRc;
	}

unsigned CInstanceListDoc::GetNextAvailableInstanceNumber()
	{
	for (unsigned nInstance = 1 ; mapIp.find(nInstance) != mapIp.end() ; nInstance++);
	return nInstance;
	}

void CInstanceListDoc::OnAppAbout() 
	{
	CAboutDlg dlgAbout(this);
	dlgAbout.DoModal();
	}

void CInstanceListDoc::OnRegister() 
	{
	CRegDlg dlgReg(this);
	dlgReg.DoModal();
	if (dlgReg.m_flValid)
		{
		cp.strRegName = dlgReg.m_strRegName;
		cp.dwRegCode1 = dlgReg.m_dwRegCode1;
		cp.flRegistered = TRUE;
		SetModifiedFlag();
		}
	}

void CInstanceListDoc::OnEventViewer() 
	{
	TRACE("CInstanceListDoc::OnEventViewer:\n");
	PROCESS_INFORMATION pi = {0};
	STARTUPINFO si = {0};
	BOOL flRc = 0;

	flRc = CreateProcess(NULL, "EVENTVWR.EXE", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	TRACE("CInstanceListDoc::OnEventViewer: err=%s\n", GetLastErrorText());
	ASSERT(flRc);
	}
