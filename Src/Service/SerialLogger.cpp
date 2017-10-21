/****************************************************************************
 SerialLogger.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
****************************************************************************/

#include "SerialLogger.h"

/****************************************************************************
 CServiceApp Implementation
****************************************************************************/

CServiceApp::CServiceApp():
	ip(&_ip),
	pp(0),
	strServiceName(""),
	hCom(NULL),
	hLogFile(NULL),
	dwTickLastRead(0),
	dwTickLastWarning(0),
	flTimeoutWarningAllowed(0),
	pComBuf(0),
	nComBufSize(0),
	flReadPending(0),	
	flReadOverlapped(0),
	dwMaxRunTicks(0),
	dEvalDaysLeft(0),
	hEventRead(0),
	timerExpire(0),
	timerReOpenPort(0),
	timerReOpenLog(0),
	timerReOpenOdbc(0),
	flRegistered(0),
	// CRecoverableError(dwServiceSpecificExitCode, dwRetryInterval, dwErrorInterval, cMaxConsecutiveErrors)
	reOpenPort(    IDERR_CANT_OPEN_PORT,      PORTRETRYINTERVAL,    DEFAULTERRORINTERVAL, 0),
	reReadPort(    IDERR_CANT_READ_PORT,      0,                    DEFAULTERRORINTERVAL, 0),
	rePortCommError(IDERR_CANT_READ_PORT,     0,                    DEFAULTERRORINTERVAL, INFINITE),
	reOpenLogFile( IDERR_CANT_OPEN_LOG_FILE,  LOGFILERETRYINTERVAL, DEFAULTERRORINTERVAL, 0),
	reWriteLogFile(IDERR_CANT_WRITE_LOG_FILE, 0,                    DEFAULTERRORINTERVAL, INFINITE),
	reOpenOdbc(    IDERR_CANT_OPEN_ODBC,      ODBCRETRYINTERVAL,    ODBCERRORINTERVAL,    0),
	reWriteOdbc(   IDERR_CANT_WRITE_ODBC,     INFINITE,             ODBCERRORINTERVAL,    INFINITE),
	reWriteOdbcNonRecoverable(IDERR_CANT_WRITE_ODBC, INFINITE,      ODBCERRORINTERVAL,    INFINITE),
	reSmtp(  IDERR_GENERIC_RECOVERABLE_ERROR, SMTPRETRYINTERVAL,    SMTPERRORINTERVAL,    INFINITE),
	strLogFilename(""),
	hTimerMinute(0),
	hTimerIdle(0),
	dwMainThreadId(0),
	hThreadMailer(0),
	dwMailerThreadId(0),
	mti(0),
	flAddPrefix(0),
	flAddPrefixToNextChunk(0),
	chLast(-1),
	chEol1('\r'), chEol2('\n'),
	dwEolMode(3),		// defaults to CR+LF
	flLineBuffer(1),
	strLineBuffer(""),
	chLineBufferLast(-1),
	henv(0),
	hdbc(0),
	hstmtWriteLine(0),
	cbLine(0)
#ifdef SERLOG_MCD
	,hTimerIdle2(0)
#endif
	{
	}

/****************************************************************************
 CServiceApp main function and initialization
****************************************************************************/

/* CServiceApp::Run:
   Call Stack at this point (service mode):
	main()
	  StartServiceCtrlDispatcher()	// system call
	    service_main()
		  ServiceStart()
		    appService.Run()

   Call Stack at this point (debug mode):
	main()
	  CmdDebugService()
	    ServiceStart()
		  appService.Run()
*/
void CServiceApp::Run(DWORD dwArgc, LPTSTR *lpszArgv)
	{
	DWORD dwRc = 0;	
	MSG msg = {0};
	HANDLE pWaitObjects[1] = {0};
	DWORD cWaitObjects = 0;

	//
	// Determine the service name and instance from lpszArgv[0]
	//

	//PrintInfoToLog("CServiceApp::Run: argc=%d", dwArgc);
	//for (int i = 0 ; i < dwArgc ; i++)
	//	PrintInfoToLog("CServiceApp::Run: argv[%d]=[%s]", i, lpszArgv[i]);

	assert(dwArgc > 0);

	strServiceName = lpszArgv[0];

	//
	// Initialize service
	//
	Initialize();

	//
	// Main loop
	//	
	for (;;)
		{
		//
        // Read all messages, removing each message as we read it
		//
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("PeekMessage returned 0x%x", msg.message);

			switch (msg.message)
				{
				case WM_READCOMPLETED:
					// This message is only used when ReadFile returns immediately
					OnReadCompleted();
					break;

				case WM_TIMER:
					OnTimer(msg.wParam);
					break;

				case WM_SETIDLETIMER:
					SetIdleTimer();
					break;

				case WM_MAILERTHREADCOMPLETE:
					assert(mti);
					delete mti;
					mti = 0;
					CloseAndNullHandle(hThreadMailer);
					break;

				case WM_SERVICEPAUSE:
					OnServicePause();
					break;
				
				case WM_SERVICECONTINUE:
					OnServiceContinue();
					break;
				
				case WM_QUIT:
				case WM_SERVICESTOP:
					goto stopevent;

				default:
					if (VerboseLevel(VERBOSE_DEBUG))
						PrintInfoToLog("Unknown message.");
					break;
				}
			}	// end of PeekMessage loop

		//
		// Wait for another message or the read event to be signaled
		//
		cWaitObjects = 0;

		// Only wait for read event if an overlapped read is pending
		if (flReadPending && flReadOverlapped)
			{
			pWaitObjects[cWaitObjects] = hEventRead;
			cWaitObjects++;
			}

		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("MsgWaitForMultipleObjects is waiting for %d objects...", cWaitObjects);

        //
		// MsgWaitForMultipleObjectsEx is called in alertable mode.  Before it returns,
		// it may call the timer APC functions.
		//
        dwRc = MsgWaitForMultipleObjectsEx(cWaitObjects, pWaitObjects, INFINITE, QS_ALLINPUT, MWMO_ALERTABLE);

		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("MsgWaitForMultipleObjects returned %d", dwRc);

        if (dwRc == WAIT_OBJECT_0 + cWaitObjects)		// we have a new message
            continue;
        else if (dwRc == WAIT_OBJECT_0)					// read event has been signaled
			{ 
			ResetEvent(hEventRead);					// is this necessary?
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("CServiceApp::Run: ReadFile has completed, calling OnReadCompleted()");
			OnReadCompleted();
			}

		} // End of the for loop
		
	//
	// Termination
	//
	stopevent:

	ReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);

#ifdef SERLOG_MCD
	FlushUnprocessedData();
	FlushInvalidData();
#endif

	CloseAndNullHandle(hLogFile);
	CloseOdbc();
	CloseAndNullHandle(hCom);
	CloseAndNullHandle(hEventRead);

	//CoUninitialize();

	PrintToLog(EVENTLOG_INFORMATION_TYPE, 0, MSG_SERVICESTOPPED, 0);
	}

/* CServiceApp::Initialize: Called from Run */
void CServiceApp::Initialize()
	{
	LONG lRc = 0;
	BOOL flRc = 0;
	DWORD dwRc = 0;
	MSG msg = {0};
	
	//
	// Reopen event logging to use instance-specific name ("SerialLogger1")
	//

    if (hEventSource)
		{
		DeregisterEventSource(hEventSource);
		hEventSource = 0;
		}	
	hEventSource = RegisterEventSource(NULL, strServiceName.c_str());

	// Report start pending status
	ReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	//
	// Force creation of a message queue for this thread
	//

	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	
	//
	// Get current thread ID; this is used by the service control thread to post messages
	//		

	dwMainThreadId = GetCurrentThreadId();
	//PrintInfoToLog("Main thread ID=%d", dwMainThreadId);

	//
	// Initialize COM
	//	
	//HRESULT hRes = CoInitialize(NULL);
	//assert(SUCCEEDED(hRes));

	//
	// Get parameters from registry and print configuration
	//

	LoadParameters();

	PrintConfig();

	//
	// Check that required registry parameters exists
	//

	if (pp->strPortName.empty())
		{
		PrintToLog("Required registry key 'PortName' does not exist or is empty.\n");
		FatalExitService(IDERR_PORT_NAME_MISSING);
		}

	if (!ip->flLogFile && !ip->flOdbc)
		{
		PrintToLog("Both registry keys 'LogFilename' and 'SQLDSN' are missing or empty.\n");
		FatalExitService(IDERR_LOG_FILENAME_MISSING);
		}

	//
	// Set SMTP error parameters (exceptions)
	//

	if (ip->flSmtp && ip->dwLogRotation == LOGROTATION_MINUTELY)
		reSmtp.dwRetryInterval = 15000;		// retry every 15 seconds

	//
	// Determine if log writing should be put into prefix mode
	//

	flAddPrefix = ip->dwTimestampFormat ? 1 : 0;

	//
	// Determine if we should write in line mode; this causes
	// writes to be buffered until EOL.
	//

	flLineBuffer = ip->flOdbc;
	
	//
	// Validate registration code and set registration status
	//

	SetRegCodeSeed(REGCODESEED);
	if (ValidateRegCode(cp.strRegName.c_str(), cp.dwRegCode1))
		{
		flRegistered = 1;
		}
	else
		{
		cp.dwRegCode1 = 0;
		cp.strRegName = SZUNLICENSEDTEXT;
		}

	//
	// If unregistered, verify that 30 day evaluation is not over
	//

	if (!IsRegistered())
		{
		BOOL flExpired = FALSE;
		FILETIME ftNow = {0}, ftInstall = {0};
		ULARGE_INTEGER uliNow = {0}, uliInstall = {0};
		ULONGLONG ullDiff = 0;
	
		// get current time
		GetSystemTimeAsFileTime(&ftNow);
		uliNow.LowPart = ftNow.dwLowDateTime;
		uliNow.HighPart = ftNow.dwHighDateTime;

		// get install time
		ftInstall.dwLowDateTime = cp.dwRegCodeA;
		ftInstall.dwHighDateTime = cp.dwRegCodeB;
		uliInstall.LowPart = ftInstall.dwLowDateTime;
		uliInstall.HighPart = ftInstall.dwHighDateTime;

		if (uliInstall.QuadPart == 0)
			{
			// set first run time to now
			uliInstall.QuadPart = uliNow.QuadPart;

			// Save first run time (install) in registry
			char *szKey = SZREGKEYCOMMON;
			HKEY hkey = 0;

			try
				{
				lRc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_SET_VALUE, &hkey);
				if (lRc != ERROR_SUCCESS)
					throw (DWORD) lRc;

				lRc = RegSetValueEx(hkey, "RegCodeA", 0, REG_DWORD,
					(CONST BYTE*) &uliInstall.LowPart, sizeof(DWORD));
				if (lRc != ERROR_SUCCESS)
					throw (DWORD) lRc;

				lRc = RegSetValueEx(hkey, "RegCodeB", 0, REG_DWORD,
					(CONST BYTE*) &uliInstall.HighPart, sizeof(DWORD));			
				if (lRc != ERROR_SUCCESS)
					throw (DWORD) lRc;

				if (hkey)
					RegCloseKey(hkey);
				}
			catch (DWORD dwErr)
				{
				SetLastError(dwErr);
				PrintToLogGLE("Unable to write to registry key %s", szKey);
				FatalExitService(IDERR_CANT_WRITE_REGKEY);
				}
			}

		if (uliNow.QuadPart >= uliInstall.QuadPart)
			{
			ullDiff = uliNow.QuadPart - uliInstall.QuadPart;
			if (EVAL100NS >= ullDiff)
				{
				dEvalDaysLeft = ((double) ((LONGLONG) (EVAL100NS - ullDiff) / 10 / 1000 / 1000))
						/ 60.0 / 60.0 / 24.0;
				}
			else
				{
				// installed more than 30 days ago
				flExpired = TRUE;
				}
			}
		else
			{
			// Install time is in future!
			flExpired = TRUE;
			}

		if (flExpired)
			{
			PrintToLog("This unlicensed version has exceeded its %u day evaluation and has expired.  "
				"To purchase this product, visit http://" SZWEBSITE ".", EVALDAYS);
			FatalExitService(IDERR_EXPIRED);
			}
		}

	//
	// Determine expiration time for unregistered version and set timer
	//	

	if (!IsRegistered())
		{
		srand(GetTickCount());
		if (rand() % 2 == 0)		// 50% of time, it will run for less than 1 hour
			dwMaxRunTicks = (DWORD) (60*60*1000 * ((double) rand() / (double) RAND_MAX));
		else	// other times, it will run for less than 14 days
			dwMaxRunTicks = (DWORD) (MAXRUNTICKS * ((double) rand() / (double) RAND_MAX));

		PrintInfoToLog("This is an unlicensed version.  It will run for %0.2f hours and then stop.  "
			"The service may be restarted after it stops provided you haven't exceeded the %u day "
			"evaluation.  You have %0.2lf days left to evaluate this product.  "
			"To purchase this product, visit "
			"http://" SZWEBSITE ".", (float) dwMaxRunTicks/1000.0/60.0/60.0, EVALDAYS, dEvalDaysLeft);

		//
		// Set timer for expiration
		//
		timerExpire = SetTimer(NULL, 0, dwMaxRunTicks, NULL);
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("timerExpire=%u", timerExpire);
		assert(timerExpire);
		}

	//
	// Initialize WinSock
	//

	if (ip->flSmtp)
		{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2,0), &wsa);
		}

	//
	// Select an appropriate com buffer size for reading
	//

	nComBufSize = pp->BaudRate / 10;
	if (nComBufSize > MAXCOMMBUFFERSIZE)
		nComBufSize = MAXCOMMBUFFERSIZE;
	else if (nComBufSize < 16)
		nComBufSize = 16;

	pComBuf = new char[nComBufSize];

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("Using com read buffer size %u", nComBufSize);

	//
	// Create event handles
	//

	hEventRead = CreateEvent(NULL, TRUE, FALSE, NULL);
	assert(hEventRead);

	memset(&overlappedRead, 0, sizeof(overlappedRead));
	overlappedRead.hEvent = hEventRead;

	//
	// Create waitable timers
	//

	hTimerMinute = CreateWaitableTimer(NULL, FALSE, NULL);
	assert(hTimerMinute);

	hTimerIdle = CreateWaitableTimer(NULL, FALSE, NULL);
	assert(hTimerIdle);

#ifdef SERLOG_MCD
	hTimerIdle2 = CreateWaitableTimer(NULL, FALSE, NULL);
	assert(hTimerIdle2);
#endif

	//
	// Open the port and begin reading; OnReadCompleted will not run until main loop
	//
	OpenPort();

	//
	// Open the log file AFTER opening port so that a log file
	// is only created if the port opens.
	//
	if (ip->flLogFile)
		OpenLogFile();

	//
	// Initialize ODBC
	//
	if (ip->flOdbc)
		OpenOdbc();

	//
	// If we are in prefix mode, set flag so prefix is added to next chunk
	//
	if (flAddPrefix)
		flAddPrefixToNextChunk = TRUE;

	//
	// Now that port, log file, and ODBC connection are opened, set max consecutive errors to
	// a high value so that errors are retried for a while before the service stops.
	//
	reOpenPort.cMaxConsecutiveErrors = PORTMAXERRORS;
	reReadPort.cMaxConsecutiveErrors = PORTMAXERRORS;
	reOpenLogFile.cMaxConsecutiveErrors = INFINITE;
	reOpenOdbc.cMaxConsecutiveErrors = INFINITE;

	//
	// Initialize idle timeout warning; start idle timer
	//

	dwTickLastRead = GetTickCount();

	//
	// If CalcTimeoutWarningAllowed() determines that time warnings are allowed,
	// it will call SetIdleTimer().
	//
	CalcTimeoutWarningAllowed();

	//
	// Start initial timers
	//

	SetMinuteTimer();

	//
	// Report running status to service control manager
	//
	{
	string s(ip->strLogFilename);
	if (ip->flOdbc)
		{
		if (!s.empty())
			s += ", ";
		s += "ODBC";
		}
	PrintToLog(EVENTLOG_INFORMATION_TYPE, 0, MSG_SERVICESTARTED, 2, pp->strPortName.c_str(), s.c_str());
	}
	ReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
	}

void CServiceApp::LoadParameters()
	{
	cp.LoadParameters();
	cp.CalcParameters();

	ip->strServiceName = strServiceName;
	if (!ip->LoadParameters())
		{
		PrintToLog("Unable to load required registry keys.\n");
		FatalExitService(IDERR_CANT_READ_INSTANCE_REGKEY);
		}

	ip->CalcParameters();

	pp = &ip->listPp[0];
	assert(pp);
	}

void CServiceApp::PrintConfig()
	{
	if (VerboseLevel(VERBOSE_CONFIGURATION))
		{
		char *szParity = 0, *szStopBits = 0;
		switch (pp->Parity)
			{
			case NOPARITY:		szParity = "None"; break;
			case ODDPARITY:		szParity = "Odd"; break;
			case EVENPARITY:	szParity = "Even"; break;
			case MARKPARITY:	szParity = "Mark"; break;
			case SPACEPARITY:	szParity = "Space"; break;
			default:			szParity = "Unknown"; break;
			}
		
		switch (pp->StopBits)
			{
			case ONESTOPBIT:	szStopBits = "1"; break;
			case ONE5STOPBITS:	szStopBits = "1.5"; break;
			case TWOSTOPBITS:	szStopBits = "2"; break;
			default:            szStopBits = "Unknown"; break;
			}

		char *szLogRotation = 0;
		switch (ip->dwLogRotation)
			{
			case LOGROTATION_NONE:		szLogRotation = "None"; break;
			case LOGROTATION_MONTHLY:	szLogRotation = "Monthly"; break;
			case LOGROTATION_WEEKLY:	szLogRotation = "Weekly"; break;
			case LOGROTATION_DAILY:		szLogRotation = "Daily"; break;
			case LOGROTATION_HOURLY:	szLogRotation = "Hourly"; break;
			case LOGROTATION_MINUTELY:	szLogRotation = "Minutely"; break;
			default:					szLogRotation = "Unknown"; break;
			}

		stringlist::iterator i;
		string strNetSendUsers("");
		for (i = pp->listNetSendUsers.begin() ; i != pp->listNetSendUsers.end() ; i++)
			{
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("NetSendUsers[%d]=[%s]", i, i->c_str());
			if (!strNetSendUsers.empty())
				strNetSendUsers += ", ";
			strNetSendUsers += *i;
			}

		string strSmtpToAddress("");
		for (i = ip->listSmtpToAddress.begin() ; i != ip->listSmtpToAddress.end() ; i++)
			{
			if (!strSmtpToAddress.empty())
				strSmtpToAddress += ", ";
			strSmtpToAddress += *i;
			}
		
		char szDayOfWeekMask[15] = "              ";
		if (pp->dwDayOfWeekMask & 0x40)
			memcpy(szDayOfWeekMask, "Su", 2);
		if (pp->dwDayOfWeekMask & 0x20)
			memcpy(szDayOfWeekMask+2, "Mo", 2);
		if (pp->dwDayOfWeekMask & 0x10)
			memcpy(szDayOfWeekMask+4, "Tu", 2);
		if (pp->dwDayOfWeekMask & 0x08)
			memcpy(szDayOfWeekMask+6, "We", 2);
		if (pp->dwDayOfWeekMask & 0x04)
			memcpy(szDayOfWeekMask+8, "Th", 2);
		if (pp->dwDayOfWeekMask & 0x02)
			memcpy(szDayOfWeekMask+10, "Fr", 2);
		if (pp->dwDayOfWeekMask & 0x01)
			memcpy(szDayOfWeekMask+12, "Sa", 2);

		char szStartTime[100], szEndTime[100];
		sprintf(szStartTime, "%d:%02d", pp->dwStartHour, pp->dwStartMin);
		sprintf(szEndTime, "%d:%02d", pp->dwEndHour, pp->dwEndMin);

		PrintInfoToLog(
			"%s version %s (%s)\r\n"
			"Copyright (C) %s by %s\r\n\r\n"
			"Licensed to: %s\r\n"
			"Service Name: %s\r\n"
			"Port: %s\r\n"
			"Log File: %s\r\n"
			"Log Rotation: %s\r\n"
			"Baud Rate: %u\r\n"
			"Byte Size: %u\r\n"
			"Parity: %s\r\n"
			"Stop Bits: %s\r\n"
			"Net Send Users: [%u] %s\r\n"
			"Custom Timeout Command: %s\r\n"
			"Timeout: %u seconds\r\n"
			"Warning Interval: %u seconds\r\n"
			"Day Of Week Mask: %s\r\n"
			"Start Time: %s\r\n"
			"End Time: %s\r\n"
			"Verbose Level: 0x%x\r\n"
			"Pause Timeout: %u milliseconds\r\n"
			"Internal Com Buffer Size: %u bytes\r\n"
			"SMTP Server: %s\r\n"
			"SMTP From Address: %s\r\n"
			"SMTP To Addresses: [%u] %s\r\n"
			"Timestamp Format: %u\r\n"
			"Max Processed Line Length (ODBC only): %u\r\n"
			"ODBC Connection String: %s\r\n"
			"ODBC SQL Write Line Statement: %s",
			SZSERVICEDISPLAYNAME, SZSERVICEVERSION, GetBuildDateTime(),
			SZCOPYRIGHTDATE, SZBANNERLINE2,
			cp.strRegName.c_str(),
			strServiceName.c_str(),
			pp->strPortName.c_str(), ip->strLogFilename.c_str(), szLogRotation,
			pp->BaudRate, pp->ByteSize, szParity,
			szStopBits, pp->listNetSendUsers.size(),
			strNetSendUsers.c_str(), pp->strTimeoutCmd.c_str(), pp->dwTimeout, pp->dwWarningInterval, szDayOfWeekMask, szStartTime, 
			szEndTime, ip->dwVerboseLevel, ip->dwPauseTimeout, pp->dwComBufferSize,
			ip->strSmtpServer.c_str(), ip->strSmtpFromAddress.c_str(), 
			ip->listSmtpToAddress.size(), strSmtpToAddress.c_str(),
			ip->dwTimestampFormat, ip->dwMaxProcessedLineLength,
			GetSafeConnectionString(ip->strSQLConnectionString).c_str(),
			ip->strSQLWriteLineStatement.c_str());
		}
	}

/****************************************************************************
 CServiceApp WM_TIMER handler function
****************************************************************************/

void CServiceApp::OnTimer(UINT nIDEvent)
	{
	BOOL flRc = 0;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::OnTimer: wParam=%d", nIDEvent);
	// TODO: replace WM_TIMER with waitable timers
	if (nIDEvent == timerReOpenLog)
		{
		assert(ip->flLogFile);
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("CServiceApp::OnTimer: re-open log; hLogFile=%p", hLogFile);
		KillTimer(NULL, timerReOpenLog);
		timerReOpenLog = 0;
		if (hLogFile == 0)
			OpenLogFile();
		}
	else if (nIDEvent == timerReOpenOdbc)
		{
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("CServiceApp::OnTimer: re-open ODBC");
		KillTimer(NULL, timerReOpenOdbc);
		timerReOpenOdbc = 0;
		if (hstmtWriteLine == 0)
			OpenOdbc();
		}
	else if (nIDEvent == timerReOpenPort)
		{
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("CServiceApp::OnTimer: re-open port");
		KillTimer(NULL, timerReOpenPort);
		timerReOpenPort = 0;
		if (hCom == 0)
			OpenPort();
		}
	else if (nIDEvent == timerExpire)
		{
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("CServiceApp::OnTimer: expire");
		PrintToLog("This is an unlicensed version.  It has run for %0.2f hours and will now stop.  "
			"The service may be restarted.  To purchase this product, visit "
			"http://" SZWEBSITE ".", (float) dwMaxRunTicks/1000.0/60.0/60.0);
		PostThreadMessage(dwMainThreadId, WM_QUIT, 0, 0);
		}
	}

/****************************************************************************
 CServiceApp serial port functions
****************************************************************************/

/* CServiceApp::OpenPort:
   Open the com port for reading and set the com port parameters.
   During initializing, any error is fatal and the service stops.
   After initializing, errors are retried.
   On failure, set timer to retry open.
   On success, start the read cycle.
*/
BOOL CServiceApp::OpenPort()
	{
	BOOL flRc = 0, flSuccess = 0;
	CRecoverableError &re = reOpenPort;
	
	try {
		//
		// Open the port
		//
		char szFullPortName[MAX_PATH] = {0};
		sprintf(szFullPortName, "\\\\.\\%s", pp->strPortName.c_str());
		assert(!flReadPending);
		assert(hCom == 0);
		// !CRF! - added GENERIC_WRITE
		hCom = CreateFile(szFullPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if (hCom == INVALID_HANDLE_VALUE)
			{
			string s = "Unable to open port " + pp->strPortName + ".  " + GetLastErrorText();
			throw s;
			}

		//
		// Set com buffer size if requested
		//
		if (pp->dwComBufferSize)
			{
			flRc = SetupComm(hCom, pp->dwComBufferSize, 16);
			if (!flRc)
				PrintToLogGLE("Unable to set com port output buffer size.");
			}

		//
		// Get current com state
		//
		DCB dcb = {0};
		dcb.DCBlength = sizeof(dcb);		// not sure if this is required
		flRc = GetCommState(hCom, &dcb);
		if (!flRc)
			PrintToLogGLE("Warning: Unable to retrieve com port state.");

		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("Initial DCB:\n%s", DCBToString(dcb).c_str());

		//
		// Overwrite com port state
		//
		dcb.BaudRate = pp->BaudRate;
		dcb.ByteSize = pp->ByteSize;
		dcb.Parity = pp->Parity;
		dcb.StopBits = pp->StopBits;
		dcb.fParity = (dcb.Parity == NOPARITY) ? FALSE : TRUE;		// enable parity checking if requested
		dcb.fAbortOnError = TRUE;

		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("New DCB:\n%s", DCBToString(dcb).c_str());

		//
		// Set comm port state
		//
		flRc = SetCommState(hCom, &dcb);
		if (!flRc)
			PrintToLogGLE("Warning: Unable to set com port state.");

		//
		// Set com timeouts used for reading
		//
		COMMTIMEOUTS ctmo = {0};
		ctmo.ReadIntervalTimeout = 1;
		flRc = SetCommTimeouts(hCom, &ctmo);
		if (!flRc)
			{
			throw string("Unable to set com port timeouts.  " + GetLastErrorText());
			}

		re.RecordSuccess();		
		
		//
		// Call StartRead() to call ReadFile overlapped.
		// If ReadFile() fails the first time, it will close the port and call
		// OpenPort().  If CreateFile succeeds, OpenPort() will then call
		// StartRead() again.  If ReadFile fails again, StartRead() will
		// call OpenPortDelayed() which will cause a delay of several seconds
		// before OpenPort() is called.
		//
		flSuccess = StartRead();
		}
	catch (string s)
		{
		re.RecordError(s.c_str());
		CloseAndNullHandle(hCom);
		OpenPortDelayed();
		}

	return flSuccess;
	}

void CServiceApp::OpenPortDelayed()
	{
	if (timerReOpenPort == 0)
		timerReOpenPort = SetTimer(NULL, 0, reOpenPort.dwRetryInterval, NULL);
	assert(timerReOpenPort);
	}

/* CServiceApp::StartRead: Called from OpenPort() */
BOOL CServiceApp::StartRead()
	{
	BOOL flRc = 0;
	DWORD nBytesRead = 0, dwError = 0;//, dwTickNow = 0;
	//CRecoverableError &re = reReadPort;

	// Loop in case ReadFile fails with a comm error
	for (;;)
		{
		assert(!flReadPending);

		flRc = ReadFile(hCom, pComBuf, nComBufSize, &nBytesRead, &overlappedRead);
		if (flRc)
			{
			// Read returned immediately with data; not overlapped; this is not expected
			flReadPending = 1;
			flReadOverlapped = 0;

			reReadPort.RecordSuccess();
			rePortCommError.RecordSuccess();

			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("CServiceApp::StartRead: Read returned immediately");
			PostThreadMessage(dwMainThreadId, WM_READCOMPLETED, 0, 0);
			return TRUE;
			}
		else
			{
			dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
				{
				// Read is overlapped
				flReadPending = 1;
				flReadOverlapped = 1;
				if (VerboseLevel(VERBOSE_DEBUG))
					PrintInfoToLog("CServiceApp::StartRead: Read returned I/O pending");
				return TRUE;
				}
			else if (dwError == ERROR_OPERATION_ABORTED)
				{
				ProcessCommError();
				// Will restart for loop
				}
			else
				{
				//
				// A read error has occured.  Close the port and reopen the port.
				// If OpenPort() successfully opens the port, it will post a message
				// to execute this function again.
				//
				flReadPending = 0;
				flReadOverlapped = 0;
				reReadPort.RecordError(string("Error reading from the port.  " + GetLastErrorText()).c_str());
				CloseAndNullHandle(hCom);

				// If this is the first consecutive error, then reopen port (and reread) immediately.
				// Otherwise, wait a few seconds and then reopen port.
				if (reReadPort.cConsecutiveErrors > 1)
					{
					OpenPortDelayed();
					return FALSE;
					}
				else
					{
					return OpenPort();
					}
				}
			}
		}
	}

/* CServiceApp::OnReadCompleted: Called by Run() when ReadFile completes overlapped I/O */
void CServiceApp::OnReadCompleted()
	{
	BOOL flRc = 0;
	DWORD cWriteErrors = 0, nBytesRead = 0, nBytesWritten = 0, dwTickNow = 0;
	DWORD dwError = 0;

	assert(flReadPending);

	if (flReadOverlapped)
		{
		assert(hCom);
		flRc = GetOverlappedResult(hCom, &overlappedRead, &nBytesRead, FALSE);
		if (!flRc)
			{
			flReadPending = 0;

			dwError = GetLastError();

			if (dwError == ERROR_OPERATION_ABORTED)
				{
				ProcessCommError();

				// Start another read immediately
				StartRead();
				return;
				}
			else
				{
				//
				// A read error has occured.  Close the port and reopen the port.
				// If OpenPort() successfully opens the port, it will post a message
				// to execute StartRead() which may cause this function to be called right away.
				//
				reReadPort.RecordError(string("Error reading from the port (delayed).  " + GetLastErrorText()).c_str());
				CloseAndNullHandle(hCom);

				// If this is the first consecutive error, then reopen port (and reread) immediately.
				// Otherwise, wait a few seconds and then reopen port.
				if (reReadPort.cConsecutiveErrors > 1)
					OpenPortDelayed();
				else
					OpenPort();
				}

			return;
			}
		}

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("Read %u bytes (flReadOverlapped=%u)", nBytesRead, flReadOverlapped);

	if (nBytesRead > 0)
		{
		reReadPort.RecordSuccess();
		rePortCommError.RecordSuccess();

		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("OnReadCompleted: hLogFile=%p", hLogFile);
		
#ifdef SERLOG_MCD

		strUnprocessedData.append((BYTE*) pComBuf, nBytesRead);
		ProcessData();

		// If unprocessed data still exists, then set a 10 second timer which will
		// flush the unprocessed data and move it to the invalid data buffer.
		if (!strUnprocessedData.empty())
			{
			// Set idle timer 2 to 10 seconds
			// convert to 100-ns intervals; negate to specify relative time
			LONGLONG llTimeout = (LONGLONG) -1 * IDLETIMER2INTERVAL * 1000 * 10;
			flRc = SetWaitableTimer(hTimerIdle2, (LARGE_INTEGER*) &llTimeout, 0, &IdleTimer2APCProc, 0, FALSE);
			assert(flRc);
			}

#else
		if (flAddPrefix)	// line mode - add prefix
			{
			/***********************************************************************************
			 When flAddPrefix is true, a prefix (ie. timestamp) is added in front of each line.
			 A prefix is added only after CR, CR+LF, or LF+CR and before the next character that
			 follows the EOL.  The prefix is only added if there is data to be printed for the
			 line, not immediately after the EOL is received.

			 chLast		pChunkBegin			desired output
			 ------     -----------         --------------
			 CR			LF					LF
			 CR			x					prefix x
			 LF			CR					CR
			 LF			x					prefix x

			***********************************************************************************/
			char *pEnd = pComBuf + nBytesRead;	// points to char after final char
			char *pChunkBegin = pComBuf;		// start of chunk is start of read buffer
			char *pChunkEnd = 0;				// points to char after final chunk char

			while (pChunkBegin < pEnd)
				{
				/* At this point, chLast will contain the last character processed (written).
				   pChunkBegin will point to the next character to process.
				   flAddPrefixToChunk will determine if prefix should be written before
				   any other data. */

				if (flAddPrefixToNextChunk)
					{
					/* Write prefix to log file. */
					if (ip->dwTimestampFormat == 1)
						{
						SYSTEMTIME st;
						GetLocalTime(&st);
						char szPrefixBuf[32];
						_snprintf(szPrefixBuf, 32, "%04u-%02u-%02u %02u:%02u:%02u.%03u: ", 
							st.wYear, st.wMonth, st.wDay,
							st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

						WriteProcessedData(szPrefixBuf, strlen(szPrefixBuf));
						}

					/* Reset add prefix to chunk flag. */
					flAddPrefixToNextChunk = FALSE;

					/* Reset last character so that next section doesn't set flAddPrefixToChunk
					   again to cause an infinite loop. */
					chLast = -1;
					}

				/* Now determine the end of the chunk.  The chunk will be the next set of data
				   that we are sure will not require a prefix to be inserted within it.  This
				   chunk will then be written to the log file. Also determine if the prefix
				   should be written at the next iteration. */

				if (chLast == chEol1)		// previous character was CR
					{
					if (*pChunkBegin == chEol2)			// followed by LF (CR+LF pair)
						{
						/* We just read CR+LF.  CR was written on last iteration.  Write LF now.
						   Prefix will be written on next iteration. */
						pChunkEnd = pChunkBegin + 1;
						flAddPrefixToNextChunk = TRUE;	// just added
						dwEolMode = 3;		// set EOL mode to CR+LF (used when reopening log file)
						}
					else								// followed by anything else
						{
						/* We just read CR+x.  Don't write x, yet.  Set add prefix flag so that
						   prefix will be written on next iteration. */
						pChunkEnd = pChunkBegin;
						flAddPrefixToNextChunk = TRUE;
						dwEolMode = 1;		// set EOL mode to CR (used when reopening log file)
						}
					}
				else if (chLast == chEol2)		// previous character was LF
					{
					if (*pChunkBegin == chEol1)			// followed by CR (LF+CR pair)
						{
						/* We just read LF+CR.  LF was written on last iteration.  Write CR now.
						   Prefix will be written on next iteration. */
						pChunkEnd = pChunkBegin + 1;
						flAddPrefixToNextChunk = TRUE;	// just added
						dwEolMode = 4;		// set EOL mode to LF+CR (used when reopening log file)
						}
					else								// followed by anything else
						{
						/* We just read LF+x.  Don't write x, yet.  Set add prefix flag so that
						   prefix will be written on next iteration. */
						pChunkEnd = pChunkBegin;
						flAddPrefixToNextChunk = TRUE;
						dwEolMode = 2;		// set EOL mode to LF (used when reopening log file)
						}
					}
#if 0				
				else if (chLast == chEol2)	// previous character was LF
					{
					/* We just read LF+x.  Don't write x, yet.  Set add prefix flag so that
					   prefix will be written on next iteration. */
					pChunkEnd = pChunkBegin;
					flAddPrefixToNextChunk = TRUE;
					dwEolMode = 2;		// set EOL mode to LF (used when reopening log file)
					}
#endif
				else
					{
					/* Either the last character written was other than CR or LF, or it was CR or LF
					   but the prefix was already written at the beginning of this iteration.
					   The last character in the chunk will be the next CR or LF or the end of
					   the read buffer, whichever comes first. */
					pChunkEnd = (char*) memchr(pChunkBegin, chEol1, chEol2, pEnd - pChunkBegin);
					if (pChunkEnd)
						/* Since pChunkEnd should point to the character following the final chunk
						   character, increment it. */
						pChunkEnd++;
					else
						/* CR or LF not found in rest of buffer.  Set chunk to rest of read buffer. */
						pChunkEnd = pEnd;
					}

				/* Write chunk to file */
				WriteProcessedData(pChunkBegin, pChunkEnd - pChunkBegin);

				/* Store last character written for next iteration or call of this function. */
				chLast = *(pChunkEnd - 1);

				/* Next iteration will start with the character after the end of the chunk just written. */
				pChunkBegin = pChunkEnd;
				}
			}
		else				// raw mode - no prefix
			WriteProcessedData(pComBuf, nBytesRead);
#endif	// SERLOG_MCD

		dwTickLastRead = GetTickCount();
		dwTickLastWarning = 0;	// reset last warning time
		SetIdleTimer();			// reset idle timer based on new last read tick
		}

	// reset read pending flag
	flReadPending = 0;

	// Start another read immediately
	StartRead();
	}

#ifdef SERLOG_MCD

//const SHORT CLogRecord::SensorIdInvalid = (SHORT) 0x7fff;

void CServiceApp::ProcessData()
	{
	CLogRecord lr;

	while (strUnprocessedData.length() >= 4)
		{
		BYTE b[4] = {strUnprocessedData[0], strUnprocessedData[1], strUnprocessedData[2], strUnprocessedData[3]};

		if (IsValidByte0(b[0]) && IsValidByte0(b[1]) &&
			IsValidByte2(b[2]) && IsValidByte2(b[3]))
			{
			FlushInvalidData();

			lr.Clear();

			SYSTEMTIME st;
			GetLocalTime(&st);
			lr.ts.Set(st);

			lr.strRawData = strUnprocessedData.substr(0, 4);
			lr.SensorId =
				(b[0] & 0x3f) |				// first byte
				((b[1] & 0x3f) << 6) |		// second byte
				((b[2] & 0x01) << 12);		// third byte
			lr.SensorValue =
				((b[2] & 0x3e) >> 1) |		// third byte
				((b[3] & 0x3f) << 5);		// fourth byte
			WriteBuffered(lr);

			strUnprocessedData.erase(0, 4);
			}
		else
			{
			// Append to invalid data buffer only if we are logging invalid data
			if (CLogRecord::SensorIdInvalid != -1)
				strInvalidData.append(strUnprocessedData, 0, 1);

			strUnprocessedData.erase(0, 1);
			}
		}
	}

string CServiceApp::LogRecordToLogFileString(CLogRecord& lr)
	{
	stringstream ss;

	if (ip->dwTimestampFormat == 1)
		{	
		char szPrefixBuf[32];
		_snprintf(szPrefixBuf, 32, "%04d-%02u-%02u %02u:%02u:%02u.%03u: ", 
			lr.ts.ts.year, lr.ts.ts.month, lr.ts.ts.day,
			lr.ts.ts.hour, lr.ts.ts.minute, lr.ts.ts.second, lr.ts.ts.fraction / 1000000);
		ss << szPrefixBuf;
		}

	if (lr.SensorId == lr.SensorIdInvalid)
		{
		ss << "INVALID: " << lr.strRawData << "\r\n";
		}
	else
		{
		ss.width(4); ss.fill('0'); ss << lr.SensorId << " ";
		ss.width(4); ss.fill('0'); ss << lr.SensorValue << " ";
		ss << lr.strRawData << "\r\n";
		}
	return ss.str();
	}

void CServiceApp::WriteBuffered(CLogRecord& lr)
	{
	//PrintInfoToLog("WriteLogRecord: lr=[%s]", LogRecordToLogFileString(lr).c_str());

	if (ip->flLogFile)
		WriteBufferedLogFile(lr);
	if (ip->flOdbc)
		WriteBufferedOdbc(lr);
	}

/* CServiceApp::FlushUnprocessedData: Forces unprocessed data to become invalid.
   Called by IdleTimer2APCProc 10 seconds after last byte is received.
*/
void CServiceApp::FlushUnprocessedData()
	{
	//stringstream ss;
	//ss << "CServiceApp::FlushUnprocessedData: invalid before: " << strInvalidData;

	// Append to invalid data buffer only if we are logging invalid data
	if (CLogRecord::SensorIdInvalid != -1)
		strInvalidData.append(strUnprocessedData);

	strUnprocessedData.erase();

	//ss << "  after: " << strInvalidData;
	//PrintInfoToLog("%s", ss.str().c_str());
	}

#else	// SERLOG_MCD

/* CServiceApp::WriteProcessedData: writes processed input to log file and/or ODBC;
   processed data includes the time stamp prefix and possibly CR+LF
*/
BOOL CServiceApp::WriteProcessedData(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
	if (ip->flLogFile)
		{
		WriteBufferedLogFile(lpBuffer, nNumberOfBytesToWrite);
		}
	if (ip->flOdbc)
		{
		WriteBufferedOdbc(lpBuffer, nNumberOfBytesToWrite);
		}
	return TRUE;
	}

#endif	// SERLOG_MCD

void CServiceApp::ProcessCommError()
	{
	DWORD lpErrors = 0;
	COMSTAT comstat = {0};

	BOOL flRc = ClearCommError(hCom, &lpErrors, &comstat);

	stringstream ss;
	if (lpErrors & CE_BREAK)
		ss << "CE_BREAK (The hardware detected a break condition)\n";
	if (lpErrors & CE_FRAME)
		ss << "CE_FRAME (The hardware detected a framing error)\n";
	if (lpErrors & CE_IOE)
		ss << "CE_IOE (An I/O error occurred during communications with the device)\n";
	if (lpErrors & CE_MODE)
		ss << "CE_MODE (The requested mode is not supported, or the hFile parameter is invalid)\n";
	if (lpErrors & CE_OVERRUN)
		ss << "CE_OVERRUN (A character-buffer overrun has occurred)\n";
	if (lpErrors & CE_RXOVER)
		ss << "CE_RXOVER (An input buffer overflow has occurred)\n";
	if (lpErrors & CE_RXPARITY)
		ss << "CE_RXPARITY (The hardware detected a parity error)\n";
	if (lpErrors & CE_TXFULL)
		ss << "CE_TXFULL (The application tried to transmit a character, but the output buffer was full)\n";
	if (lpErrors & (CE_OVERRUN | CE_RXOVER))
		ss << "The input buffer is full with " << comstat.cbInQue << " unread bytes.  "
			"Try increasing the input buffer by setting the registry value "
			"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\" << strServiceName <<
			"\\Parameters\\Port1\\ComBufferSize to " <<
			RoundToPage(comstat.cbInQue * 2) << ".";
	
	if (ss.str().empty())
		{
		ss << "0x";
		ss.width(8); ss.fill('0'); ss << std::hex;
		ss << lpErrors << " (Unknown error)\n";
		}

	string s = "The following comm errors were encountered while reading from the port:\n";
	s += ss.str();

	rePortCommError.RecordError(s.c_str());
	}

/****************************************************************************
 CServiceApp log file functions
****************************************************************************/

/* CServiceApp::OpenLogFile:
   Open the log file for writing.
   During initializing, any error is fatal and the service stops.
   After initializing, errors are retried.
   On failure, set timer to retry open.
   On success, write any queued data.
*/
BOOL CServiceApp::OpenLogFile()
	{
	BOOL flRc = 0, flSuccess = 0;
	CRecoverableError &re = reOpenLogFile;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::OpenLogFile");

	assert(ip->flLogFile);
	assert(hLogFile == NULL);

	// always calculate log file rotation before opening the log file
	CalcLogFileRotation();

	// first try opening for read + write
	hLogFile = CreateFile(strLogFilename.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hLogFile == INVALID_HANDLE_VALUE)
		{
		// opening for read+write failed; try again for write only
		string strGLE = GetLastErrorText();
		CloseAndNullHandle(hLogFile);

		hLogFile = CreateFile(strLogFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hLogFile == INVALID_HANDLE_VALUE)
			{
			string s = "Unable to open the log file " + strLogFilename + " for writing.  " +
				GetLastErrorText();		

			CloseAndNullHandle(hLogFile);

			//
			// If service is continuing, we must report the status.
			// If there was an error opening the log file, the log open retry interval is 
			// lowered from 60 s to 1 s.  Upon success, the interval is set back to 60s.
			//
			if (IsServiceContinuing() && re.cMaxConsecutiveErrors)
				{
				re.dwRetryInterval = CONTINUERETRYINTERVAL;
				re.cMaxConsecutiveErrors = CONTINUEMAXERRORS;
				ReportStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, re.dwRetryInterval*2);
				}
			re.RecordError(s.c_str());
			}
		else
			{
			PrintToLog("Warning: The log file could not be opened for read and write.  "
				"It has been successfully opened for write only.  %s", strGLE.c_str());			
			}
		}

	if (hLogFile)
		{
		// Move to end of file to append to it
		DWORD dwRc = SetFilePointer(hLogFile, 0, 0, FILE_END);

		/* If we are in prefix mode, then make sure we start on a new line.
		   Also, always write prefix before writing any data. */
		if (flAddPrefix)
			{
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("OpenLogFile: SetFilePointer=%u", dwRc);

			if (dwRc)
				{
				/* The log file has at least one character; read up to last 1 KB of it. */
				const unsigned cMaxBufSize = 1024;
				char pBuf[cMaxBufSize];

				unsigned cBufSize = (dwRc >= cMaxBufSize) ? cMaxBufSize : dwRc;

				if (VerboseLevel(VERBOSE_DEBUG))
					PrintInfoToLog("OpenLogFile: cBufSize=%u", cBufSize);

				dwRc = SetFilePointer(hLogFile, -cBufSize, 0, FILE_END);
				DWORD nNumberOfBytesRead = 0;
				flRc = ReadFile(hLogFile, pBuf, cBufSize, &nNumberOfBytesRead, 0);

				/* Attempt to locate last CR or LF in buffer. */
				char *p = (char*) memrchr(pBuf, chEol1, chEol2, nNumberOfBytesRead);

				if (VerboseLevel(VERBOSE_DEBUG))
					PrintInfoToLog("OpenLogFile: last EOL found at pBuf[%d]", p - pBuf);

				BOOL flWriteEol = FALSE;
				
				/* Determine if read succeeded. */
				if (flRc)
					{
					/* Read succeeded. */
					if (p)
						{
						/* CR or LF found.
						   Determine EOL mode by examining CR or LF. */
						if (p > pBuf && *(p-1) == chEol1 && *p == chEol2)
							dwEolMode = 3;		// CR+LF
						else if (p > pBuf && *(p-1) == chEol2 && *p == chEol1)
							dwEolMode = 4;		// LF+CR
						else if (*p == chEol2)
							dwEolMode = 2;
						else if (*p == chEol1)
							dwEolMode = 1;

						if (VerboseLevel(VERBOSE_DEBUG))
							PrintInfoToLog("OpenLogFile: dwEolMode=%u", dwEolMode);

						/* Determine if last character is CR or LF. */
						if (p != pBuf + nNumberOfBytesRead - 1)
							/* Last character is not CR or LF. */
							flWriteEol = TRUE;
						/*     Else last character is CR or LF. Don't write EOL. */
						}
					else
						/* CR or LF not found. Write EOL. */
						flWriteEol = TRUE;
					}
				/*     Else read failed. Don't write EOL. */

				/* Determine EOL string and write it if necessary. */
				if (flWriteEol)
					{
					string s("");
					switch (dwEolMode)
						{
						case 1:		// CR
							s += chEol1;
							break;
						case 2:		// LF
							s += chEol2;
							break;
						case 4:		// LF+CR
							s += chEol2;
							s += chEol1;
							break;
						default:	// CR+LF
							s += chEol1;
							s += chEol2;
							break;
						}

					/* Write EOL. */
					if (VerboseLevel(VERBOSE_DEBUG))
						PrintInfoToLog("OpenLogFile: writing EOL=(%x,%x,[%s])", s[0], s[1], s.c_str());

#ifdef SERLOG_MCD
					flRc = WriteLogFile(s);
#else
					flRc = WriteLogFile(s.c_str(), s.size());
#endif
					}
				}
			//flAddPrefixToNextChunk = TRUE;
			}

		// write queued data if any
		if (!strLogFileBuf.empty())
			{
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("CServiceApp::OpenLogFile: writing %d bytes from log buf", strLogFileBuf.length());

#ifdef SERLOG_MCD
			flRc = WriteLogFile(strLogFileBuf);
#else
			flRc = WriteLogFile(strLogFileBuf.data(), strLogFileBuf.length());
#endif
			if (flRc)
				{
				strLogFileBuf.erase();
				flSuccess = TRUE;
				}
			else
				{
				//
				// Could not write to log file immediately after opening it.
				// Close log file and reopen in a few seconds.
				//
				CloseAndNullHandle(hLogFile);
				}
			}
		else
			flSuccess = TRUE;

		if (flSuccess)
			{
			re.RecordSuccess();
			if (IsServiceContinuing() && re.cMaxConsecutiveErrors)
				{
				re.dwRetryInterval = LOGFILERETRYINTERVAL;
				re.cMaxConsecutiveErrors = INFINITE;
				PrintToLog(EVENTLOG_INFORMATION_TYPE, 0, MSG_SERVICECONTINUED, 0);
				ReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
				}		
			}
		}

	if (!flSuccess)
		OpenLogFileDelayed();

	return flSuccess;
	}

void CServiceApp::OpenLogFileDelayed()
	{
	assert(ip->flLogFile);
	if (timerReOpenLog == 0)
		timerReOpenLog = SetTimer(NULL, 0, reOpenLogFile.dwRetryInterval, NULL);
	assert(timerReOpenLog);
	}

#ifdef SERLOG_MCD

void CServiceApp::WriteBufferedLogFile(CLogRecord& lr)
	{
	string str = LogRecordToLogFileString(lr);

	if (hLogFile)		// only write to log if it is open (ie. service is not paused)
		{
		BOOL flRc = WriteLogFile(str);
		if (!flRc)
			{
			//
			// A write error has occured.  Close log file, write to log buffer,
			// and then try to re-open the log file.
			//
			CloseAndNullHandle(hLogFile);
			strLogFileBuf.append(str);
			OpenLogFile();
			}
		}
	else
		strLogFileBuf.append(str);	
	}

BOOL CServiceApp::WriteLogFile(string& str)
	{
	BOOL flRc = 0;
	DWORD nBytesWritten = 0;
	CRecoverableError &re = reWriteLogFile;

	assert(hLogFile);
	flRc = WriteFile(hLogFile, str.data(), str.length(), &nBytesWritten, NULL);
	if (flRc)
		re.RecordSuccess();
	else
		re.RecordError(string("Error writing to the log file.  " + GetLastErrorText()).c_str());
	return flRc;
	}

#else	// not SERLOG_MCD

/* CServiceApp::WriteBufferedLogFile: writes processed input to log file or memory buffer */
BOOL CServiceApp::WriteBufferedLogFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
	BOOL flRc;

	if (hLogFile)		// only write to log if it is open (ie. service is not paused)
		{
		flRc = WriteLogFile(lpBuffer, nNumberOfBytesToWrite);
		if (!flRc)
			{
			//
			// A write error has occured.  Close log file, write to log buffer,
			// and then try to re-open the log file.
			//
			CloseAndNullHandle(hLogFile);
			strLogFileBuf.append((BYTE*) lpBuffer, nNumberOfBytesToWrite);
			OpenLogFile();
			}
		}
	else
		strLogFileBuf.append((BYTE*) lpBuffer, nNumberOfBytesToWrite);

	return TRUE;
	}

BOOL CServiceApp::WriteLogFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
	BOOL flRc = 0;
	DWORD nBytesWritten = 0;
	CRecoverableError &re = reWriteLogFile;

	assert(hLogFile);
	flRc = WriteFile(hLogFile, lpBuffer, nNumberOfBytesToWrite,	&nBytesWritten, NULL);
	if (flRc)
		re.RecordSuccess();
	else
		re.RecordError(string("Error writing to the log file.  " + GetLastErrorText()).c_str());
	return flRc;
	}

#endif	// SERLOG_MCD

/****************************************************************************
 CServiceApp ODBC functions
****************************************************************************/

BOOL CServiceApp::OpenOdbc()
	{
	BOOL flRc = 0, flSuccess = 0, iRc = 0;
	char szSql[1024] = {0};
	SQLRETURN sr;
	CRecoverableError &re = reOpenOdbc;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::OpenOdbc: begin");

	CloseOdbc();

	try
		{	
		// Allocate environment handle
		sr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
		AssertSQLEnv(sr, henv);

		// Set the ODBC version environment attribute
		sr = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0); 
		AssertSQLEnv(sr, henv);

		// Allocate connection handle
		sr = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 
		AssertSQLEnv(sr, henv);

		// Set login timeout to 5 seconds.
		sr = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER) 5, 0);
		AssertSQLDBC(sr, hdbc);

		// Connect to data source
		//sr = SQLConnect(hdbc, (SQLCHAR*) ip->strSQLDSN.c_str(), SQL_NTS,
		//		(SQLCHAR*) ip->strSQLUserName.c_str(), SQL_NTS, (SQLCHAR*) ip->strSQLPassword.c_str(), SQL_NTS);
		const SQLSMALLINT MaxConnectionStringBufSize = 4096;
		SQLSMALLINT ConnectionStringBufSize = 0;
		SQLCHAR ConnectionString[MaxConnectionStringBufSize] = {0};
		sr = SQLDriverConnect(hdbc, NULL, (SQLCHAR*) ip->strSQLConnectionString.c_str(), SQL_NTS, 
			ConnectionString, MaxConnectionStringBufSize,
			&ConnectionStringBufSize, SQL_DRIVER_NOPROMPT);
		AssertSQLDBC(sr, hdbc);

		// Allocate statement handle
		sr = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtWriteLine);
		AssertSQLDBC(sr, hdbc);

		// Prepare SQL statement
		sr = SQLPrepare(hstmtWriteLine, (SQLCHAR*) ip->strSQLWriteLineStatement.c_str(), SQL_NTS);
		AssertSQLStmt(sr, hstmtWriteLine);

		// Bind parameters for SQL statement
#ifdef SERLOG_MCD
		unsigned paramnum = 1;
		sr = SQLBindParameter(hstmtWriteLine, paramnum++, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
			23, 3, &lrOdbc.ts.ts, 0, 0);
		AssertSQLStmt(sr, hstmtWriteLine);

		sr = SQLBindParameter(hstmtWriteLine, paramnum++, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT,
			0, 0, &lrOdbc.SensorId, 0, 0);
		AssertSQLStmt(sr, hstmtWriteLine);

		sr = SQLBindParameter(hstmtWriteLine, paramnum++, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT,
			0, 0, &lrOdbc.SensorValue, 0, 0);
		AssertSQLStmt(sr, hstmtWriteLine);
#else
		sr = SQLBindParameter(hstmtWriteLine, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR,
			ip->dwMaxProcessedLineLength, 0, (SQLPOINTER) 1, 0, &cbLine);
		AssertSQLStmt(sr, hstmtWriteLine);
#endif

		//
		// Write queued data if any
		//
#ifndef SERLOG_MCD
#error this section must be changed
#endif
		if (!listOdbcRecordBuf.empty())
			{
			if (VerboseLevel(VERBOSE_DEBUG))
				PrintInfoToLog("CServiceApp::OpenOdbc: writing %d records from the record buf", listOdbcRecordBuf.size());

			while (!listOdbcRecordBuf.empty())
				{
				CLogRecord& lr = listOdbcRecordBuf.front();	// get first string
				iRc = WriteOdbc(lr);						// write it to ODBC
				if (iRc == 1)
					{
					// An ODBC connection error (SqlState 08xxx) has occured.
					// Close connection, and then try to reconnect to ODBC
					CloseOdbc();
					OpenOdbcDelayed();
					return FALSE;
					}
				else if (iRc == 2)
					{
					// A non-recoverable ODBC error has occured.
					// The line is discarded.
					reWriteOdbcNonRecoverable.RecordError("A non-recoverable ODBC error has been encountered while "
						"writing a record from the record buffer.  The line has been discarded.");
					listOdbcRecordBuf.pop_front();	// remove first string so we don't try again
					}
				else
					{
					listOdbcRecordBuf.pop_front();	// on success, remove first string
					}
				}
			}
		}
	catch (COdbcException e)
		{
		re.RecordError(string("Error initializing ODBC connection: " + 
			GetSQLErrorText(e.HandleType, e.h)).c_str());
		OpenOdbcDelayed();
		return FALSE;
		}

	re.RecordSuccess();
	return TRUE;
	}

void CServiceApp::OpenOdbcDelayed()
	{
	PrintInfoToLog("CServiceApp::OpenOdbcDelayed");
	if (timerReOpenOdbc == 0)
		timerReOpenOdbc = SetTimer(NULL, 0, reOpenOdbc.dwRetryInterval, NULL);
	assert(timerReOpenOdbc);
	}

BOOL CServiceApp::CloseOdbc()
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::CloseOdbc: begin");

	// Free handles if previously allocated
	if (hstmtWriteLine)
		{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmtWriteLine);
		hstmtWriteLine = 0;
		}
	if (hdbc)
		{
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = 0;
		}
	if (henv)
		{
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = 0;
		}
	return TRUE;
	}

#ifdef SERLOG_MCD

void CServiceApp::WriteBufferedOdbc(CLogRecord& lr)
	{
	if (hstmtWriteLine)
		{
		int iRc = WriteOdbc(lr);
		if (iRc == 1)
			{
			// An ODBC connection error (SqlState 08xxx) has occured.
			// Close connection, write to log buffer,
			// and then try to reconnect to ODBC
			CloseOdbc();
			listOdbcRecordBuf.push_back(lr);
			OpenOdbc();
			}
		else if (iRc == 2)
			{
			// A non-recoverable ODBC error has occured.
			// The line is discarded.
			reWriteOdbcNonRecoverable.RecordError("A non-recoverable ODBC error has been encountered while "
				"writing a line.  The line has been discarded.");
			}
		}
	else
		listOdbcRecordBuf.push_back(lr);
	}

/* CServiceApp::WriteOdbc:
   returns 0 on success, 1 on recoverable error (reconnect may correct problem),
   2 on non-recoverable error
*/
int CServiceApp::WriteOdbc(CLogRecord& lr)
	{
	CRecoverableError& re = reWriteOdbc;

	//if (VerboseLevel(VERBOSE_DEBUG))
	//	PrintInfoToLog("CServiceApp::WriteOdbc: s=[%s]", s.c_str());

	assert(hstmtWriteLine);
	
	// Write to ODBC driver

	try
		{
		SQLRETURN sr = 0, sr1 = 0;
		SQLPOINTER pToken = 0;

		// Copy CLogRecord to lrOdbc which is where SQLExecute will look for the query parameters
		lrOdbc = lr;

		// Execute statement		
		sr1 = SQLExecute(hstmtWriteLine);
		AssertSQLStmt(sr1, hstmtWriteLine);

		// Close results cursor
		sr1 = SQLFreeStmt(hstmtWriteLine, SQL_CLOSE);
		AssertSQLStmt(sr1, hstmtWriteLine);
		}
	catch (COdbcException e)
		{
		stringlist listSqlState;
		string s = GetSQLErrorText(e.HandleType, e.h, &listSqlState);
		re.RecordError(string("Error writing to ODBC data source.").c_str());
		if (IsRecoverableSQLError(&listSqlState))
			return 1;
		else
			return 2;
		}

	re.RecordSuccess();
	return 0;
	}

#else	// not SERLOG_MCD

/* CServiceApp::WriteBufferedOdbc: writes processed input to ODBC or memory buffer */
BOOL CServiceApp::WriteBufferedOdbc(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
	BOOL flRc = 0;
	int iRc = 0;
	int iChunkEndPos = 0;

	strLineBuffer.append((char*) lpBuffer, nNumberOfBytesToWrite);

	string strEol("");
	strEol += (char) chEol1;
	strEol += (char) chEol2;

	for (;;)
		{
		// If first character is the second character of an EOL pair, then discard it
		if (!strLineBuffer.empty() &&
				((chLineBufferLast == chEol1 && strLineBuffer[0] == chEol2) ||
				 (chLineBufferLast == chEol2 && strLineBuffer[0] == chEol1)))
			{
			strLineBuffer.erase(strLineBuffer.begin());		// erase first character (LF)
			chLineBufferLast = -1;
			}

		// Try to find first EOL in buffer
		iChunkEndPos = strLineBuffer.find_first_of(strEol);

		// If EOL not found, then exit loop
		if (iChunkEndPos == string::npos)
			break;

		// EOL found, write line to ODBC or ODBC buffer
		string s(strLineBuffer, 0, iChunkEndPos); 

		if (hstmtWriteLine)
			{
			iRc = WriteOdbc(s);
			if (iRc == 1)
				{
				// An ODBC connection error (SqlState 08xxx) has occured.
				// Close connection, write to log buffer,
				// and then try to reconnect to ODBC
				CloseOdbc();
				//PrintInfoToLog("CServiceApp::WriteBufferedOdbc: appending to line buf due to error");
				listOdbcLineBuf.push_back(s);
				OpenOdbc();
				}
			else if (iRc == 2)
				{
				// A non-recoverable ODBC error has occured.
				// The line is discarded.
				reWriteOdbcNonRecoverable.RecordError("A non-recoverable ODBC error has been encountered while "
					"writing a line.  The line has been discarded.");
				}
			}
		else
			{
			//PrintInfoToLog("CServiceApp::WriteBufferedOdbc: appending to line buf");
			listOdbcLineBuf.push_back(s);
			}

		chLineBufferLast = strLineBuffer[iChunkEndPos];
		strLineBuffer.erase(0, iChunkEndPos + 1);
		}
	return TRUE;
	}

/* CServiceApp::WriteOdbc:
   returns 0 on success, 1 on recoverable error (reconnect may correct problem),
   2 on non-recoverable error
*/
int CServiceApp::WriteOdbc(string &s)
	{
	CRecoverableError &re = reWriteOdbc;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::WriteOdbc: s=[%s]", s.c_str());

	assert(hstmtWriteLine);
	
	// Write to ODBC driver

	try
		{
		SQLRETURN sr = 0, sr1 = 0;
		SQLPOINTER pToken = 0;
		SQLINTEGER cbData = (s.size() > ip->dwMaxProcessedLineLength) ? ip->dwMaxProcessedLineLength : s.size();
		cbLine = SQL_LEN_DATA_AT_EXEC(cbData);

		// Execute statement		
		sr1 = SQLExecute(hstmtWriteLine);
		AssertSQLStmt(sr1, hstmtWriteLine);

		// Pass parameter
		while (sr1 == SQL_NEED_DATA)
			{
			sr1 = SQLParamData(hstmtWriteLine, &pToken);
			AssertSQLStmt(sr1, hstmtWriteLine);
			//PrintInfoToLog("CServiceApp::WriteLine: pToken=%u, sr1=%d", pToken, sr1);
			if (sr1 == SQL_NEED_DATA)
				{
				sr = SQLPutData(hstmtWriteLine, (SQLPOINTER) s.data(), cbData);
				AssertSQLStmt(sr, hstmtWriteLine);
				}
			}

		// Close results cursor
		sr1 = SQLFreeStmt(hstmtWriteLine, SQL_CLOSE);
		AssertSQLStmt(sr1, hstmtWriteLine);
		}
	catch (COdbcException e)
		{
		stringlist listSqlState;
		string s = GetSQLErrorText(e.HandleType, e.h, &listSqlState);
		re.RecordError(string("Error writing line to ODBC data source: " + s).c_str());
		if (IsRecoverableSQLError(&listSqlState))
			return 1;
		else
			return 2;
		}

	re.RecordSuccess();
	return 0;
	}

#endif	// SERLOG_MCD

/****************************************************************************
 CServiceApp log rotation functions
****************************************************************************/

/* CServiceApp::CalcLogFileRotation:
   Determines if a log rotation is necessary.  If so, calculates
   log file range and filename.
   Returns TRUE if log rotation is necessary.
*/
BOOL CServiceApp::CalcLogFileRotation()
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::CalcLogFileRotation: begin");

	assert(ip->flLogFile);

	// Get current local time and determine if outside current log rotation time span
	LONGLONG llCurrentTime = GetLocalFileTimeQuad();
	if (llCurrentTime >= llRotateRangeBegin &&
		llCurrentTime < llRotateRangeEnd)
		{
		return FALSE;	// still within current range
		}

	// close current log file
	CloseAndNullHandle(hLogFile);

	// mail log file
	MailLogFile();

	// Calc log rotation begin/end times and log file name
	CalcCurrentLogFileRotation(*ip, strLogFilename, llRotateRangeBegin, llRotateRangeEnd);

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::CalcLogFileRotation: file=[%s], begin=%s, end=%s", 
			strLogFilename.c_str(), QuadTimeToString(llRotateRangeBegin).c_str(),
			QuadTimeToString(llRotateRangeEnd).c_str());
	return TRUE;
	}

/* RotateLogFile:
   Rotates the log file if necessary.  Called from minute timer.
*/
void CServiceApp::RotateLogFile()
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::RotateLogFile: begin");

	assert(ip->flLogFile);

	if (CalcLogFileRotation())
		{
		if (VerboseLevel(VERBOSE_DEBUG))
			PrintInfoToLog("CServiceApp::RotateLogFile: re-opening log file");

		// open new log file if service is not paused
		if (!IsServicePaused())
			OpenLogFile();
		}
	}

/****************************************************************************
 CServiceApp log e-mailing functions
****************************************************************************/

void CServiceApp::MailLogFile()
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::MailLogFile: begin, file=%s", strLogFilename.c_str());

	assert(ip->flLogFile);

	if (ip->flSmtp && !strLogFilename.empty())
		{
		if (hThreadMailer)
			{
			PrintToLog("The mailer thread is still attempting to send the previous log file %s.  " 
				"The current log file %s will not be e-mailed.",
				mti->strLogFilename.c_str(), strLogFilename.c_str());
			}
		else
			{
			assert(mti == 0);
			mti = new CMailThreadInfo;

			mti->strLogFilename = strLogFilename;
			PrepareMailThreadInfo(*mti, *ip);

			hThreadMailer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &MailerThreadStart, 
				mti, 0, &dwMailerThreadId);
			assert(hThreadMailer);
			//PrintInfoToLog("CServiceApp::MailLogFile: created thread id %d", dwMailerThreadId);
			}
		}

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::MailLogFile: end");
	}

DWORD WINAPI CServiceApp::MailerThreadStart(CMailThreadInfo *mti)
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::MailerThreadStart: begin");

	CRecoverableError &re = appService.reSmtp;

	re.RecordSuccess();		// reset consecutive error count to 0

	while (re.cConsecutiveErrors < SMTPMAXERRORS)
		{
		CServiceSmtp mail;
		if (mail.Connect((char*) mti->strSmtpServer.c_str()))
			{
			int iRc = mail.SendMessage(mti->msg);
			mail.Close();
			if (iRc)
				{
				stringstream ss;
				ss << "Unable to send SMTP message.  Error code is " << iRc << ".";
				re.RecordError(ss.str().c_str());
				}
			else
				{
				re.RecordSuccess();
				if (VerboseLevel(VERBOSE_SMTPSUMMARY))
					PrintInfoToLog("Successfully e-mailed log file %s.", mti->strLogFilename.c_str());
				break;
				}
			}
		else
			re.RecordError("Unable to connect to SMTP server.");

		Sleep(re.dwRetryInterval);
		}

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::MailerThreadStart: end");

	PostThreadMessage(appService.dwMainThreadId, WM_MAILERTHREADCOMPLETE, (WPARAM) mti, 0);
	return 0;
	}

/****************************************************************************
 CServiceApp idle warning functions
****************************************************************************/

/* CServiceApp::SetIdleTimer:
   Called after successful port read or change in warning allowed status.
   If the warnings are not currently allowed, this function does nothing.
   Otherwise, this function calculates the time until the next idle warning.  If
   the time is now or in the past, it issues the warning.  Then it sets a
   timer which will call this function after the next idle warning is
   expected.
*/
void CServiceApp::SetIdleTimer()
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::SetIdleTimer: begin: flTimeoutWarningAllowed=%u, "
					   "    dwTickLastWarning=%u, dwWarningInterval=%d, dwTickLastRead=%u",
			flTimeoutWarningAllowed, dwTickLastWarning, pp->dwWarningInterval, dwTickLastRead);

	//
	// Issue idle timeout warnings if necessary and/or set timer to test for timeout
	// in the future.
	// First, determine whether timeouts are enabled.
	//
	if (pp->dwTimeout)
		{
		//
		// Timeout warnings are enabled.
		//
		if (dwTickLastWarning == 0 || pp->dwWarningInterval)
			{
			//
			// First warning hasn't been issued, or warnings are issued at intervals.
			// Another warning is possible in the future.
			//
			if (flTimeoutWarningAllowed)
				{
				//
				// Timeout warnings are allowed during this time of day and day of week.
				// Calculate time until next warning.
				//
				DWORD dwTimeUntilWarning = 0;
				DWORD dwTickNow = GetTickCount();

				if (dwTickLastWarning)
					{
					//
					// First warning has already been issued.  Use last warning time
					// to determine time until next warning.
					//
					if (dwTickLastWarning + pp->dwWarningInterval * 1000 > dwTickNow)
						dwTimeUntilWarning = dwTickLastWarning + pp->dwWarningInterval * 1000 - dwTickNow;
					else
						dwTimeUntilWarning = 0;
					}
				else
					{
					//
					// First warning has not been issued.  Use last read time
					// to determine time until next warning.
					//
					if (dwTickLastRead + pp->dwTimeout * 1000 > dwTickNow)
						dwTimeUntilWarning = dwTickLastRead + pp->dwTimeout * 1000 - dwTickNow;
					else
						dwTimeUntilWarning = 0;
					}

				if (VerboseLevel(VERBOSE_DEBUG))
					PrintInfoToLog("CServiceApp::SetIdleTimer: dwTimeUntilWarning=%u", dwTimeUntilWarning);

				//
				// Give warning now if necessary and then return
				//
				if (dwTimeUntilWarning == 0)
					{
					GiveIdleWarning();
					//Sleep(1000);

					//
					// Post message to set idle timer.  Don't call SetIdleTimer() directly
					// to avoid recursion and to ensure that bad timing parameters
					// don't create infinite recursion.
					//
					PostThreadMessage(dwMainThreadId, WM_SETIDLETIMER, 0, 0);
					return;
					}

				//
				// Set new timer
				//

				// convert to 100-ns intervals; negate to specify relative time
				LONGLONG llTimeUntilWarning = (LONGLONG) -1000 * 10 * dwTimeUntilWarning;
				BOOL flRc = SetWaitableTimer(hTimerIdle, (LARGE_INTEGER*) &llTimeUntilWarning, 0, &IdleTimerAPCProc, 0, FALSE);
				assert(flRc);

				if (VerboseLevel(VERBOSE_DEBUG))
					PrintInfoToLog("CServiceApp::SetIdleTimer: set timer for %u ms", dwTimeUntilWarning);
				}
			}
		}
	}

void CALLBACK CServiceApp::IdleTimerAPCProc(LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue, DWORD dwTimerHighValue)
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::IdleTimerAPCProc: %x:%x", dwTimerHighValue, dwTimerLowValue);
	appService.SetIdleTimer();
	}

/*
	CalcTimeoutWarningAllowed
	Sets flTimeoutWarningAllowed based on time of day, day of week, etc.
*/
void CServiceApp::CalcTimeoutWarningAllowed()
	{
	SYSTEMTIME stLocal = {0};
	double fHourNow = 0;

	//
	// Get current local time and convert hours/minutes to a float
	//
	GetLocalTime(&stLocal);
	fHourNow = (float) stLocal.wHour + (stLocal.wMinute / 60.0);						
	
	//
	// Determine if the current day of week and hour fall within the specified range
	//
	if ((fHourNow >= pp->fStartHour) &&
		(fHourNow <= pp->fEndHour) &&
		((0x40 >> stLocal.wDayOfWeek) & pp->dwDayOfWeekMask))
		flTimeoutWarningAllowed = TRUE;
	else
		flTimeoutWarningAllowed = FALSE;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::CalcTimeoutWarningAllowed: flTimeoutWarningAllowed=%u",
			flTimeoutWarningAllowed);

	//
	// If flTimeoutWarningAllowed just changed to true from false, then
	// SetIdleTimer() must be called.  However, it doesn't hurt to call it always.
	//
	SetIdleTimer();	
	}

void CServiceApp::GiveIdleWarning()
	{
	DWORD dwTickNow = GetTickCount();
	DWORD dwIdleSeconds = (dwTickNow - dwTickLastRead) / 1000;
	stringstream ss("");

	if (VerboseLevel(VERBOSE_TIMEOUT))		
		{
		ss << dwIdleSeconds;
		PrintToLog(EVENTLOG_WARNING_TYPE, 0, MSG_TIMEOUT, 1, ss.str().c_str());
		ss.str(string(""));		// erase string so we can use it again
		}

	// send timeout warning using net send
	ss << "WARNING: " SZSERVICEDISPLAYNAME " has not received data for "
		<< dwIdleSeconds << " seconds.";
	NetSendMessage(ss.str().c_str(), pp->listNetSendUsers);

	// run custom timeout command
	if (!pp->strTimeoutCmd.empty())
		{
		PROCESS_INFORMATION pi = {0};
		STARTUPINFO si = {0};
		CUpdatableString us(pp->strTimeoutCmd);

		si.cb = sizeof(STARTUPINFO);
		BOOL flRc = CreateProcess(NULL, us.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!flRc)
			PrintToLogGLE("CreateProcess cannot start the custom timeout command.");
		}

	dwTickLastWarning = dwTickNow ? dwTickNow : -1;		// 0 is a reserved value meaning never
	}

/****************************************************************************
 CServiceApp minute timer functions
****************************************************************************/

void CServiceApp::SetMinuteTimer()
	{
	SYSTEMTIME st;
	LARGE_INTEGER li, liNextMin;
	GetSystemTime(&st);		// get current UTC time
	st.wSecond = 0;			// clear seconds and milliseconds to get last minute 
	st.wMilliseconds = 0;
	SystemTimeToFileTime(&st, (FILETIME*) &li);
	liNextMin.QuadPart = li.QuadPart + 60 * 1000 * 1000 * 10;		// increment by 60 seconds (FILETIME is in 100 nanoseconds)
																	// to get next minute

	// Set minute timer; repeats every 1 minute
	BOOL flRc = SetWaitableTimer(hTimerMinute, &liNextMin, 60 * 1000, &MinuteTimerAPCProc, 0, FALSE);
	assert(flRc);

	// determine how many seconds until timer is first signaled
	LONGLONG llDiff = liNextMin.QuadPart - GetSystemFileTimeQuad();
	llDiff /= 10 * 1000 * 1000;
	int iDiff = llDiff;
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::SetMinuteTimer: minute timer will signal in %d seconds", iDiff);
	}


void CALLBACK CServiceApp::MinuteTimerAPCProc(LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue, DWORD dwTimerHighValue)
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::MinuteTimerAPCProc: %x:%x", dwTimerHighValue, dwTimerLowValue);
	//PostThreadMessage(dwMainThreadId, WM_MINUTETIMER, dwTimerLowValue, dwTimerHighValue);
	appService.OnMinuteTimer(dwTimerLowValue, dwTimerHighValue);
	}

void CServiceApp::OnMinuteTimer(DWORD dwTimerLowValue, DWORD dwTimerHighValue)
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::OnMinuteTimer: %x:%x", dwTimerHighValue, dwTimerLowValue);

	//
	// Convert UTC file time to system time.
	// This is used later to determine if the timer should be reset.
	//
	FILETIME ftSystem = {0};
	SYSTEMTIME stSystem = {0};
	ftSystem.dwLowDateTime = dwTimerLowValue;
	ftSystem.dwHighDateTime = dwTimerHighValue;
	FileTimeToSystemTime(&ftSystem, &stSystem);
	
	if (VerboseLevel(VERBOSE_DEBUG))
		{
		// display local time
		FILETIME ftLocal = {0};
		SYSTEMTIME stLocal = {0};
		FileTimeToLocalFileTime(&ftSystem, &ftLocal);
		FileTimeToSystemTime(&ftLocal, &stLocal);
		PrintInfoToLog("CServiceApp::OnMinuteTimer: %u/%u/%u %02u:%02u:%02u.%03u", 
			stLocal.wMonth, stLocal.wDay, stLocal.wYear,
			stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
		}

	// Rotate log file if necessary
	if (ip->flLogFile)
		RotateLogFile();

	CalcTimeoutWarningAllowed();

#ifdef SERLOG_MCD
	// At least once a minute, flush invalid data to the database and log file
	FlushInvalidData();
#endif

	// If timer is not called within the first 5 seconds of a minute,
	// then reset the timer.
	if (stSystem.wSecond >= 5)
		SetMinuteTimer();
	}

/****************************************************************************
 CServiceApp service control functions
****************************************************************************/

//
// Pause the service; close the log file
//
void CServiceApp::OnServicePause()
	{
	ReportStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 3000);

	CloseAndNullHandle(hLogFile);

	PrintToLog(EVENTLOG_INFORMATION_TYPE, 0, MSG_SERVICEPAUSED, 0);
	ReportStatus(SERVICE_PAUSED, NO_ERROR, 0);					
	}

//
// Continue the service; reopen the log file
//
void CServiceApp::OnServiceContinue()
	{
	DWORD nBytesWritten = 0;
	BOOL flRc = 0;

	ReportStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 3000);

	//
	// Attempt to open the log file with OpenLogFile().  If it is successful, it will report
	// that the service is running.  Otherwise, it will report continue pending
	// and set a timer to reopen every 1 s.
	//
	if (ip->flLogFile)
		OpenLogFile();
	}

/****************************************************************************
 CServiceApp MCD-specific functions
****************************************************************************/

#ifdef SERLOG_MCD

void CALLBACK CServiceApp::IdleTimer2APCProc(LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue, DWORD dwTimerHighValue)
	{
	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CServiceApp::IdleTimerAPCProc2: %x:%x", dwTimerHighValue, dwTimerLowValue);
	appService.FlushUnprocessedData();
	}

/* CServiceApp::FlushInvalidData: If any data is in the invalid data buffer, a log record
   is created.  This is called by ProcessData, OnMinuteTimer and Run (on service stop).
*/
void CServiceApp::FlushInvalidData()
	{
	//stringstream ss;
	//ss << "CServiceApp::FlushInvalidData: invalid: " << strInvalidData;
	//PrintInfoToLog("%s", ss.str().c_str());

	if (!strInvalidData.empty())
		{
		CLogRecord lr;

		SYSTEMTIME st;
		GetLocalTime(&st);
		lr.ts.Set(st);

		lr.strRawData = strInvalidData;
		lr.SensorId = lr.SensorIdInvalid;
		lr.SensorValue = strInvalidData.length();
		WriteBuffered(lr);

		strInvalidData.erase();
		}
	}

#endif	// SERLOG_MCD

/****************************************************************************
 CRecoverableError Implementation
****************************************************************************/

CRecoverableError::CRecoverableError(
									 DWORD dwServiceSpecificExitCode, 
									 DWORD dwRetryInterval, 
									 DWORD dwErrorInterval, 
									 DWORD cMaxConsecutiveErrors):
	dwRetryInterval(dwRetryInterval),
	dwErrorInterval(dwErrorInterval),
	cMaxConsecutiveErrors(cMaxConsecutiveErrors),
	dwServiceSpecificExitCode(dwServiceSpecificExitCode),
	cErrors(0),
	cConsecutiveErrors(0),
	dwTickLastErrorReport(0)
	{
	// set last error report tick to maximum distance
	// away from current tick count.
	dwTickLastErrorReport = GetTickCount() + 0x8000000;
	}

/*
	Called whenever a recoverable error is encountered.
*/
BOOL CRecoverableError::RecordError(const char *szErr)
	{
	cErrors++;
	cConsecutiveErrors++;

	if (VerboseLevel(VERBOSE_DEBUG))
		PrintInfoToLog("CRecoverableError::RecordError: szErr=%s, cConsecutiveErrors=%u\n",
			szErr, cConsecutiveErrors);

	// If too many consecutive errors, then stop service
	if (cConsecutiveErrors > cMaxConsecutiveErrors)
		{
		stringstream ss;
		ss << szErr << "\r\n";
		ss << "This type of error has occured " << cErrors << ((cErrors == 1) ? " time" : " times")
		   << " since the service has started.\r\n";
		if (cMaxConsecutiveErrors > 0)
			ss << "The service is stopping because there have been too many "
				"consecutive errors.";
		else
			ss << "The service is stopping.";

		ReportError(ss.str().c_str());
		FatalExitService(dwServiceSpecificExitCode);
		}

	// Determine if we should report the error
	DWORD dwTickNow = GetTickCount();
	if (ElapsedTicks(dwTickNow, dwTickLastErrorReport) >= dwErrorInterval)
		{
		stringstream ss;
		ss << szErr << "\r\n";
		ss << "This type of error has occured " << cErrors << ((cErrors == 1) ? " time" : " times")
		   << " since the service has started.\r\n";
		if (dwRetryInterval == 0)
			ss << "The service will retry again." ;
		else if (dwRetryInterval != INFINITE)
			{
			if (cMaxConsecutiveErrors == INFINITE)
				ss << "The service will retry again every " << dwRetryInterval/1000 
				   << " seconds.";
			else
				ss << "The service will retry again every " << dwRetryInterval/1000 
				   << " seconds for up to " << RetrySecondsLeft() << " seconds."
				      "  If unsuccessful after this time, the service will stop.";
			}

		ReportError(ss.str().c_str());
		dwTickLastErrorReport = dwTickNow;
		return TRUE;
		}
	else
		return FALSE;
	}

void CRecoverableError::ReportError(const char *szErr)
	{
	::ReportError(szErr);
	}

inline BOOL CRecoverableError::RecordSuccess()
	{
	if (cConsecutiveErrors)
		{
		cConsecutiveErrors = 0;
		return TRUE;
		}
	else
		return FALSE;
	}

/****************************************************************************
 CServiceSmtp Implementation
****************************************************************************/

int CServiceSmtp::SmtpError(int nError, LPTSTR pszErr)
	{
	PrintToLog("SMTP error %d: %s", nError, pszErr);
	return 0;
	}

void CServiceSmtp::SmtpCommandResponse(LPTSTR pszCmd, int nResponse, LPTSTR pszResponse)
	{
	if (VerboseLevel(VERBOSE_SMTPDETAIL))
		{
		PrintInfoToLog("SMTP Sent: [%s]\r\n"
			"SMTP Recv: [%s]", pszCmd ? pszCmd : "", pszResponse ? pszResponse : "");
		}
	}

/****************************************************************************
 CLogRecord implementation
****************************************************************************/

#ifdef SERLOG_MCD

CLogRecord::CLogRecord():
	SensorId(0),
	SensorValue(0)
	{
	}

void CLogRecord::Clear()
	{
	ts.Clear();
	strRawData.erase();
	SensorId = 0;
	SensorValue = 0;
	}

#endif
