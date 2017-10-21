/****************************************************************************
 Service.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
****************************************************************************/

#include "SerialLogger.h"

/****************************************************************************
 Generic Service global variables
****************************************************************************/

SERVICE_STATUS ssStatus = {0};
SERVICE_STATUS_HANDLE sshStatusHandle = 0;
BOOL bDebug = FALSE;
DWORD dwErr = 0;
TCHAR szErr[256];
HANDLE hEventSource = NULL;

/*****************************************************************************
 SerialLogger global variables
*****************************************************************************/

CServiceApp appService;

/*****************************************************************************
 main() and service control functions
*****************************************************************************/

int main(int argc, char **argv)
	{
	int RetVal = 0;

	//
	// Initialize event logging using base service name ("SerialLogger")
	//	
	hEventSource = RegisterEventSource(NULL, SZSERVICENAME);

    printf("%s version %s (%s)\n"
		   "Copyright (C) %s by %s\n\n",
		SZSERVICEDISPLAYNAME, SZSERVICEVERSION, GetBuildDateTime(), SZCOPYRIGHTDATE, SZBANNERLINE2);

	// Initialize reg code seed
	SetRegCodeSeed(REGCODESEED);

	//
	// Parse command-line parameters to determine if we should enter debug mode
	// 
	for (int i = 0 ; i < argc ; i++)
		{
		if (stricmp(argv[i], "/debug") == 0)
			bDebug = TRUE;
		}
    
	if (bDebug)
		{
        CmdDebugService(argc, argv);
		}
	else
		{	
		// Create dispatch table; lpServiceName is ignored for this type of service
		// The service name is in service_main argv[0].
		SERVICE_TABLE_ENTRY dispatchTable[] = 
			{{SZSERVICENAME, (LPSERVICE_MAIN_FUNCTION) service_main}, {NULL, NULL}};

		// the service control manager may be starting the service
		// so we must call StartServiceCtrlDispatcher
		printf("StartServiceCtrlDispatcher being called.\n" );
		printf("This may take several seconds.  Please wait...\n" );

		if (!StartServiceCtrlDispatcher(dispatchTable))
			{
			RetVal = 1;
			PrintToLogGLE("StartServiceCtrlDispatcher failed.");
			}
		}

	//
	// Terminate event logging
	//
    if (hEventSource)
		{
		DeregisterEventSource(hEventSource);
		hEventSource = 0;
		}

	return RetVal;
	}

/* service_main: Service entry point called by service control dispatcher */
void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
	{
	// Note: The name of the service is in lpszArgv[0]

	//PrintInfoToLog("service_main: argc=%d", dwArgc);
	//for (int i = 0 ; i < dwArgc ; i++)
	//	PrintInfoToLog("service_main: argv[%d]=[%s]", i, lpszArgv[i]);

	assert(dwArgc > 0);
	assert(lpszArgv);
	assert(lpszArgv[0]);
	assert(lpszArgv[0][0]);
    
	// register our service control handler:
    sshStatusHandle = RegisterServiceCtrlHandler(lpszArgv[0], service_ctrl);
	assert(sshStatusHandle);

	ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ssStatus.dwServiceSpecificExitCode = 0;

	// report the status to the service control manager.
	if (ReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000))
		{
		// start the actual service		
		ServiceStart(dwArgc, lpszArgv);
		}

	// report the stopped status to the service control manager.
	//
    ReportStatus(SERVICE_STOPPED, dwErr, 0);
	}

/* service_ctrl: Called by the service control dispatcher */
void WINAPI service_ctrl(DWORD dwCtrlCode)
	{
    // Handle the requested control code.
    //
    switch(dwCtrlCode)
		{
        case SERVICE_CONTROL_STOP:
            ServiceStop();
            break;

		case SERVICE_CONTROL_PAUSE:
			if (ssStatus.dwCurrentState == SERVICE_RUNNING)
				ServicePause();
			break;

		case SERVICE_CONTROL_CONTINUE:
			if (ssStatus.dwCurrentState == SERVICE_PAUSED)
				ServiceContinue();
			break;

        case SERVICE_CONTROL_INTERROGATE:
			ReportStatus(ssStatus.dwCurrentState, NO_ERROR, 0);
            break;

        // invalid control code
        //
        default:
            break;
		}
	}

/*****************************************************************************
 Service debug functions
*****************************************************************************/

/* CmdDebugService: Replaces service_main in debug mode */
void CmdDebugService(int argc, char **argv)
	{
    DWORD dwArgc;
    LPTSTR *lpszArgv;
	LPTSTR szArgv0Default = SZSERVICENAME "1";

	if (argc < 3)
		{
		// defaults to "SerialLogger1" as service parameters
		dwArgc = 1;
		lpszArgv = &szArgv0Default;
		}
	else
		{
		// skip "seriallogger.exe /debug"
		dwArgc   = (DWORD) argc - 2;
		lpszArgv = argv + 2;
		}

    printf("Debugging %s.\n", lpszArgv[0]);

    SetConsoleCtrlHandler(ControlHandler, TRUE);

    ServiceStart(dwArgc, lpszArgv);
	}

/* ControlHandler: Used for debug mode */
BOOL WINAPI ControlHandler (DWORD dwCtrlType)
	{
	static BOOL flRunning = 1;
    switch (dwCtrlType)
		{
        case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
            printf("Stopping service.\n");
            ServiceStop();
            return TRUE;
            break;
        case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
			flRunning = !flRunning;
			if (flRunning)		
				{
				printf("Continuing service.\n");
				ServiceContinue();
				}
			else
				{
				printf("Pausing service.\n");
				ServicePause();
				}
			return TRUE;
			break;
		}
    return FALSE;
	}

/*****************************************************************************
 Service start, stop, pause, continue functions
*****************************************************************************/

/* Service Start: Called from service_main() */
void ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv)
	{
	appService.Run(dwArgc, lpszArgv);
	}

/* Service Stop: Called from the service control handler service_ctrl() */
void ServiceStop()
	{
	if (appService.dwMainThreadId)
		PostThreadMessage(appService.dwMainThreadId, WM_SERVICESTOP, 0, 0);
	}

/* Service Pause: Called from the service control handler service_ctrl() */
void ServicePause()
	{
	if (appService.dwMainThreadId)
		PostThreadMessage(appService.dwMainThreadId, WM_SERVICEPAUSE, 0, 0);
	}

/* Service Continue: Called from the service control handler service_ctrl() */
void ServiceContinue()
	{
	if (appService.dwMainThreadId)
		PostThreadMessage(appService.dwMainThreadId, WM_SERVICECONTINUE, 0, 0);
	}

/*****************************************************************************
 Service utility functions
*****************************************************************************/

BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint,
						 DWORD dwServiceSpecificExitCode)
	{
    static DWORD dwCheckPoint = 1;
    BOOL fResult = TRUE;

    ssStatus.dwCurrentState = dwCurrentState;
    ssStatus.dwWin32ExitCode = dwWin32ExitCode;
	ssStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
    ssStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        ssStatus.dwControlsAccepted = 0;
    else
        ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

    if (dwCurrentState == SERVICE_START_PENDING || dwCurrentState == SERVICE_STOP_PENDING ||
        dwCurrentState == SERVICE_CONTINUE_PENDING || dwCurrentState == SERVICE_PAUSE_PENDING)		
        ssStatus.dwCheckPoint = dwCheckPoint++;
    else
        ssStatus.dwCheckPoint = 0;

    if (bDebug)
		fResult = TRUE;
	else
		{
        // Report the status of the service to the service control manager.
        if (sshStatusHandle && !(fResult = SetServiceStatus(sshStatusHandle, &ssStatus)))
			{
            PrintToLogGLE("ReportStatusToSCMgr:SetServiceStatus failed.");
			}
		}
    return fResult;
	}

void ReportError(const char *szErr)
	{
	PrintToLog("%s", szErr);

	string s = string("The " SZSERVICEDISPLAYNAME " service has reported the following error message:\n\n") +
		szErr + "\n\nCheck the Event Log for further details.";
	NetSendMessage(s.c_str(), appService.pp->listNetSendUsers);
	}

/* NetSendMessage: Sends the message to everybody in the user list */
void NetSendMessage(const char *szMessage, stringlist &listUsers)
	{
	PROCESS_INFORMATION pi = {0};
	STARTUPINFO si = {0};
	BOOL flRc = 0;
	CUpdatableString us("");

	si.cb = sizeof(STARTUPINFO);

	// Send a "net send" message to each user
	stringlist::iterator i;		
	for (i = listUsers.begin() ; i != listUsers.end() ; i++)
		{
		us = "net.exe send " + *i + " \"" + szMessage + "\"";
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("NetSendMessage: cmd=[%s]", us.c_str());
		flRc = CreateProcess(NULL, us.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL,
			&si, &pi);
		if (!flRc)
			PrintToLogGLE("CreateProcess cannot start the NET SEND command");
		}
	}

void FatalExitService(DWORD dwServiceSpecificExitCode)
	{
	PrintToLog(EVENTLOG_INFORMATION_TYPE, 0, MSG_SERVICESTOPPED, 0);
	ReportStatus2(SERVICE_STOPPED, ERROR_SERVICE_SPECIFIC_ERROR, 0, dwServiceSpecificExitCode);
	ExitProcess(dwServiceSpecificExitCode ? dwServiceSpecificExitCode : ERROR_SERVICE_SPECIFIC_ERROR);
	}

#define PRINTBUFFERSIZE                         10240

void PrintToLog(WORD wType, WORD wCategory, DWORD dwEventID, WORD wNumStrings, ...)
	{
	char *lpszStrings[16];
	if (wNumStrings > 16)
		wNumStrings = 16;
	va_list va;
	va_start(va, wNumStrings);
	for (unsigned i = 0 ; i < wNumStrings ; i++)
		lpszStrings[i] = (char*) va_arg(va, char*);
    if (hEventSource)
		ReportEvent(hEventSource, wType, wCategory, dwEventID, NULL, wNumStrings, 0, (const char **) lpszStrings, NULL);
	if (bDebug)
		printf("PrintToLog message %u.\n", dwEventID);
	}

void PrintToLog(char *fmt, ...)
	{
	va_list va;
	va_start(va, fmt);
	char buf[PRINTBUFFERSIZE];
    char *lpszStrings[1] = {buf};
	_vsnprintf(buf, PRINTBUFFERSIZE, fmt, va);
    if (hEventSource)
	    ReportEvent(hEventSource, EVENTLOG_ERROR_TYPE, 0, MSG_GENERICERROR, NULL, 1, 0, (const char **) lpszStrings, NULL);
	if (bDebug)
		puts(buf);
	}

void PrintToLogGLE(char *fmt, ...)
	{
	DWORD dwErr = GetLastError();

	va_list va;
	va_start(va, fmt);
	char buf[PRINTBUFFERSIZE] = {0}, szErr[512] = {0};
    char *lpszStrings[1] = {buf};

	GetErrorText(szErr, 512, dwErr);

	_vsnprintf(buf, PRINTBUFFERSIZE, fmt, va);
	buf[PRINTBUFFERSIZE-1] = 0;
	_snprintf(buf + strlen(buf), PRINTBUFFERSIZE - strlen(buf), "  %s", szErr);
	buf[PRINTBUFFERSIZE-1] = 0;
    if (hEventSource)
		ReportEvent(hEventSource, EVENTLOG_ERROR_TYPE, 0, MSG_GENERICERROR, NULL, 1, 0, (const char **) lpszStrings, NULL);
	if (bDebug)
		puts(buf);
	}

void PrintInfoToLog(char *fmt, ...)
	{
	va_list va;
	va_start(va, fmt);
	char buf[PRINTBUFFERSIZE];
    char *lpszStrings[1] = {buf};
	_vsnprintf(buf, PRINTBUFFERSIZE, fmt, va);
	buf[PRINTBUFFERSIZE-1] = 0;
    if (hEventSource)
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, MSG_GENERICINFO, NULL, 1, 0, (const char **) lpszStrings, NULL);
	if (bDebug)
		puts(buf);
	}

void _Assert(char *exp, char *file, unsigned line)
	{
	char szLine[10], szError[10];
	PrintToLog(EVENTLOG_ERROR_TYPE, 0, MSG_ASSERTFAILED, 4, exp, file, itoa(line,szLine,10), itoa(GetLastError(),szError,10));
	if (bDebug)
		fprintf(stderr, "Assertion failed: expression %s in file %s on line %s.\nGetLastError() is %s.\n",
			exp, file, szLine, szError);
	FatalExitService(IDERR_ASSERTION_FAILED);
	} 

void _Assert(char *exp, char *file, unsigned line, char *szdispexp, DWORD dwdispexp)
	{
	char szLine[10], szError[10], szDispExp[10];
	PrintToLog(EVENTLOG_ERROR_TYPE, 0, MSG_ASSERTFAILED2, 6, exp, file, itoa(line,szLine,10), 
		itoa(GetLastError(),szError,10), szdispexp, itoa(dwdispexp, szDispExp, 10));
	if (bDebug)
		{
		fprintf(stderr, "Assertion failed: expression %s in file %s on line %s.\nGetLastError() is %s.\n",
			exp, file, szLine, szError);
		fprintf(stderr, "%s = %s\n", szdispexp, szDispExp);
		}
	FatalExitService(IDERR_ASSERTION_FAILED);
	} 
