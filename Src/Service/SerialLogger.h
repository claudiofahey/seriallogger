/*****************************************************************************
 SerialLogger.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#pragma warning(disable : 4786)		// disable warning about identifier being too long for debug info

#include <windows.h>

#include <string>
#include <list>
#include <deque>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <atlbase.h>

#include "..\Common\SerialLoggerCommon.h"
#include "..\Common\CommonResource.h"
#include "..\Common\BuildTimestamp.h"
#include "SerialLoggerMsg.h"		// messages

/*****************************************************************************
 Serial Logger defines
*****************************************************************************/

#define WM_SERVICESTOP				(WM_USER + 1)
#define	WM_SERVICEPAUSE				(WM_USER + 2)
#define WM_SERVICECONTINUE			(WM_USER + 3)
#define WM_READCOMPLETED			(WM_USER + 4)
#define WM_SETIDLETIMER				(WM_USER + 5)
#define WM_MAILERTHREADCOMPLETE		(WM_USER + 6)

#define MAXCOMMBUFFERSIZE			(128 * 1024)	// max size of comm buffer (128 KB = 1280 Kbps * 1 sec)

#define ABSOLUTETIMECHECKPERIOD	60000		// current time will be recalculated at least this often (ms)

// Recoverable Error Parameters
#define DEFAULTERRORINTERVAL	(1*60000)	// milliseconds between error messages (1 min)

#define PORTRETRYINTERVAL		10000		// milliseconds between retries (10 sec)
#define PORTMAXERRORS			(6*30)		// max consecutive errors until service stops (30 min)

#define LOGFILERETRYINTERVAL	10000		// milliseconds between retries (10 sec)

#define CONTINUERETRYINTERVAL	1000		// while continuing, retry log open with this interval
#define CONTINUEMAXERRORS		120			// while continuing, max consecutive errors (2 min)

#define SMTPRETRYINTERVAL		(10*60000)	// milliseconds between SMTP retries (10 min)
#define SMTPERRORINTERVAL		(30*60000)	// milliseconds between error messages (30 min)
#define SMTPMAXERRORS			4			// max number of retries for sending e-mail; e-mail is not sent afterwards

#define ODBCRETRYINTERVAL		(1*60000)	// milliseconds between ODBC retries (1 min)
#define ODBCERRORINTERVAL		(5*60000)	// milliseconds between error messages (5 min)

#ifdef SERLOG_MCD
#define IDLETIMER2INTERVAL		10000		// Idle timer 2 (used to flush unprocessed data) in milliseconds
#endif

/*****************************************************************************
 Generic Service defines
*****************************************************************************/

//#define TRACESTATUS
#ifdef TRACESTATUS
 #define ReportStatus(state, exitcode, waithint)						\
	(PrintToLog("ReportStatusToSCMgr: %s, line %d", #state, __LINE__),	\
	ReportStatusToSCMgr(state, exitcode, waithint, 0))
 #define ReportStatus2(state, exitcode, waithint, ssexitcode)			\
	(PrintToLog("ReportStatusToSCMgr: %s, line %d", #state, __LINE__),	\
	ReportStatusToSCMgr(state, exitcode, waithint, ssexitcode))
#else
 #define ReportStatus(state, exitcode, waithint)						\
	ReportStatusToSCMgr(state, exitcode, waithint, 0)
 #define ReportStatus2(state, exitcode, waithint, ssexitcode)			\
	ReportStatusToSCMgr(state, exitcode, waithint, ssexitcode)
#endif

#define CHECKPOINT()	PrintToLog("CHECKPOINT: File %s, Line %d", __FILE__, __LINE__)

typedef std::basic_string<BYTE> bstring;

/*****************************************************************************
 Forward class declarations
*****************************************************************************/

class CServiceApp;
class CLogRecord;
class CSqlTimestamp;

/*****************************************************************************
 Type definitions
*****************************************************************************/


/*****************************************************************************
 Global variables
*****************************************************************************/

extern SERVICE_STATUS ssStatus;
extern CServiceApp appService;
extern HANDLE hEventSource;

/*****************************************************************************
 Global function prototypes
*****************************************************************************/

void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
void WINAPI service_ctrl(DWORD dwCtrlCode);

VOID CmdDebugService(int argc, char **argv);
BOOL WINAPI ControlHandler (DWORD dwCtrlType);

void ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
void ServiceStop();
void ServicePause();
void ServiceContinue();

BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint,
						 DWORD dwServiceSpecificExitCode);
void ReportError(const char *szErr);
void NetSendMessage(const char *szMessage, stringlist &listUsers);
void FatalExitService(DWORD dwServiceSpecificExitCode = 0);
void PrintToLog(WORD wType, WORD wCategory, DWORD dwEventID, WORD wNumStrings, ...);
void PrintToLog(char *fmt, ...);
void PrintToLogGLE(char *fmt, ...);
void PrintInfoToLog(char *fmt, ...);
void _Assert(char *exp, char *file, unsigned line);
void _Assert(char *exp, char *file, unsigned line, char *szdispexp, DWORD dwdispexp);

string DCBToString(DCB &dcb);
LPTSTR GetErrorText(LPTSTR lpszBuf, DWORD dwSize, DWORD dwErr);
string GetErrorText(DWORD dwErr);
BOOL CloseAndNullHandle(HANDLE &h);
void* __cdecl memchr(const void *buf, int chr1, int chr2, size_t cnt);
void* __cdecl memrchr(const void *buf, int chr1, int chr2, size_t cnt);

//#ifdef SERLOG_MCD
std::ostream& operator<<(std::ostream& os, bstring& bstr);
//#endif

/*****************************************************************************
 Class definitions
*****************************************************************************/

class CUpdatableString : public string
	{
	public:
	CUpdatableString(string str);
	CUpdatableString(char *sz);
	~CUpdatableString();
	char *GetBuffer(unsigned long nMinBufLength = 0);
	CUpdatableString &ReleaseBuffer();

	protected:
	char *szBuf;
	unsigned long nBufLength;	
	};

class CSqlTimestamp
	{
	public:
	SQL_TIMESTAMP_STRUCT ts;

	CSqlTimestamp();
	void Clear();
	void Set(SYSTEMTIME& st);
	};

#ifdef SERLOG_MCD

class CLogRecord
	{
	public:
	CSqlTimestamp ts;
	SHORT SensorId;
	SHORT SensorValue;
	bstring strRawData;

	enum {SensorIdInvalid = 0x7fff};		// set to -1 to disable logging of invalid data

	CLogRecord();
	void Clear();
	};

#endif

class CRecoverableError
	{
	public:
	CRecoverableError(DWORD dwServiceSpecificExitCode = IDERR_GENERIC_RECOVERABLE_ERROR, 
		DWORD dwRetryInterval = 0, 
		DWORD dwErrorInterval = DEFAULTERRORINTERVAL, 
		DWORD cMaxConsecutiveErrors = 0);
	BOOL RecordError(const char *szErr);	// called by service to record all errors
	void ReportError(const char *szErr);	// called by RecordError to notify user through Event Log
											//   and Net Send
	inline BOOL RecordSuccess();			// called by service to record all successes
											//   this is necessary to reset the consecutive error count
	inline DWORD RetrySeconds() {return dwRetryInterval * cMaxConsecutiveErrors / 1000;}
	inline LONG RetrySecondsLeft()
		{return dwRetryInterval * ( (LONG) cMaxConsecutiveErrors - cConsecutiveErrors + 1) / 1000;}

	DWORD dwRetryInterval;			// number of milliseconds between retries;
									// 0 = will retry at unspecified time, INFINITE = never retry
	DWORD dwErrorInterval;			// number of milliseconds between error reports
	DWORD cMaxConsecutiveErrors;	// maximum consecutive errors until service stops;
									// INFINITE = never stop service due to this error
	DWORD dwServiceSpecificExitCode;	// service status is set to this in case of a fatal error

	DWORD dwTickLastErrorReport;	// tick of last ReportError() call
	DWORD cErrors;					// number of errors since service start
	DWORD cConsecutiveErrors;		// number of errors since last success
	};

class CServiceApp
	{
	public:
	CServiceApp();
	void Run(DWORD dwArgc, LPTSTR *lpszArgv);
	void Initialize();
	void LoadParameters();
	void PrintConfig();
	BOOL OpenPort();
	void OpenPortDelayed();
	BOOL OpenLogFile();
	void OpenLogFileDelayed();
	void ProcessCommError();
	BOOL OpenOdbc();
	void OpenOdbcDelayed();
	BOOL CloseOdbc();
	BOOL StartRead();
	void OnReadCompleted();
	BOOL WriteProcessedData(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
	void SetIdleTimer();
	void GiveIdleWarning();
	void CalcTimeoutWarningAllowed();
	void OnTimer(UINT nIDEvent);
	void OnServicePause();
	void OnServiceContinue();
	BOOL CalcLogFileRotation();
	void RotateLogFile();
	static void CALLBACK MinuteTimerAPCProc(LPVOID lpArgToCompletionRoutine,
		DWORD dwTimerLowValue, DWORD dwTimerHighValue);
	void OnMinuteTimer(DWORD dwTimerLowValue, DWORD dwTimerHighValue);
	void SetMinuteTimer();
	static void CALLBACK IdleTimerAPCProc(LPVOID lpArgToCompletionRoutine,
		DWORD dwTimerLowValue, DWORD dwTimerHighValue);
	void MailLogFile();
	static DWORD WINAPI MailerThreadStart(CMailThreadInfo *mti);

	inline BOOL IsServiceContinuing() {return ssStatus.dwCurrentState == SERVICE_CONTINUE_PENDING;}
	inline BOOL IsServicePaused() {return ssStatus.dwCurrentState == SERVICE_PAUSED;}
	inline BOOL IsServiceStarting() {return ssStatus.dwCurrentState == SERVICE_START_PENDING;}
	inline BOOL IsRegistered() {return flRegistered;};

	CCommonParameters cp;
	CInstanceParameters *ip, _ip;
	CPortParameters *pp;
	
	string strServiceName;			// service name with instance number (i.e. "SerialLogger1")
	HANDLE hCom;					// handle to com port
	HANDLE hLogFile;				// handle to log file
	DWORD dwTickLastRead;			// time of last received data
	DWORD dwTickLastWarning;		// time of last timeout warning (0 = never)
	BOOL flTimeoutWarningAllowed;	// true if timeout warnings are allowed during this time of day and day of week
	char *pComBuf;					// com buffer;
	unsigned nComBufSize;			// com buffer size
	BOOL flReadPending;				// true if read is pending or completed
	BOOL flReadOverlapped;			// true if last read is overlapped
	DWORD dwMaxRunTicks;			// program expires after this many ticks
	double dEvalDaysLeft;			// number of eval days left
	LONGLONG llRotateRangeBegin;	// first time in log rotation
	LONGLONG llRotateRangeEnd;		// last time in log rotation
	HANDLE hEventRead;				// event used to signal overlapped read completion
	UINT timerExpire;				// timer for shareware expiration
	UINT timerReOpenPort;			// timer for re-reading port after read error
	UINT timerReOpenLog;			// timer for re-opening the log file after open error
	UINT timerReOpenOdbc;			// timer for re-opening the ODBC connection after open error
	string strLogFileBuf;			// buffer to hold com data when log file is unavailable
	OVERLAPPED overlappedRead;		// overlapped structure for reading
	CRecoverableError reOpenPort;
	CRecoverableError reReadPort;
	CRecoverableError rePortCommError;
	CRecoverableError reOpenLogFile;
	CRecoverableError reWriteLogFile;
	CRecoverableError reOpenOdbc;
	CRecoverableError reWriteOdbc;
	CRecoverableError reWriteOdbcNonRecoverable;
	string strLogFilename;			// current output log filename
	HANDLE hTimerMinute;
	HANDLE hTimerIdle;
	DWORD dwMainThreadId;			// thread id of main thread
	HANDLE hThreadMailer;
	DWORD dwMailerThreadId;			// thread id of mailer thread
	CMailThreadInfo *mti;
	CRecoverableError reSmtp;
	BOOL flAddPrefix;				// set to true to add timestamp prefix to each line
	BOOL flAddPrefixToNextChunk;	// true if prefix must be added before any more data is written
	int chLast;						// stores last character from previous buffer (-1 = none)
	char chEol1, chEol2;
	DWORD dwEolMode;				// character(s) that end each line: 1=CR, 2=LF, 3=CR+LF, 4=LF+CR
	BOOL flLineBuffer;				// true if output is written line by line instead of immediately
	string strLineBuffer;			// line buffer
	int chLineBufferLast;			// last character written from last line buffer
	SQLHENV henv;					// ODBC environment handle
	SQLHDBC hdbc;					// ODBC connection handle
	SQLHSTMT hstmtWriteLine;		// ODBC statement handle
	long cbLine;
	std::deque<string> listOdbcLineBuf;

#ifdef SERLOG_MCD
	void ProcessData();
	string LogRecordToLogFileString(CLogRecord& lr);
	void WriteBuffered(CLogRecord& lr);
	void WriteBufferedLogFile(CLogRecord& lr);
	BOOL WriteLogFile(string& str);
	void WriteBufferedOdbc(CLogRecord& lr);
	int WriteOdbc(CLogRecord& lr);
	static void CALLBACK IdleTimer2APCProc(LPVOID lpArgToCompletionRoutine,
		DWORD dwTimerLowValue, DWORD dwTimerHighValue);
	void FlushInvalidData();
	void FlushUnprocessedData();

	bstring strUnprocessedData;		// characters received but insufficient to make a complete packet
	bstring strInvalidData;			// invalid characters since last successful packet
	std::deque<CLogRecord> listOdbcRecordBuf;
	CLogRecord lrOdbc;
	HANDLE hTimerIdle2;				// 10 second timer for invalidating unprocessed data
#else
	BOOL WriteBufferedLogFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
	BOOL WriteLogFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
	BOOL WriteBufferedOdbc(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
	int WriteOdbc(string &s);
#endif

	private:
	BOOL flRegistered;
	};

class CServiceSmtp: public CSmtp
	{
	virtual int SmtpError(int nError, LPTSTR pszErr);
	virtual void SmtpCommandResponse(LPTSTR pszCmd, int nResponse, LPTSTR pszResponse);
	};

/*****************************************************************************
 Inline functions
*****************************************************************************/

inline BOOL VerboseLevel(DWORD dw)
	{
	return (appService.ip->dwVerboseLevel & dw) ? TRUE : FALSE;
	}

/* ElapsedTicks:
   Returns the number of ticks elapsed from
   tick0 to tick1.  It is required that tick0 represent
   a tick taken before tick1.  This function is valid
   as long as the total elapsed time is less than 2^32 ms (49.7 days).
*/
inline DWORD ElapsedTicks(DWORD dwTick1, DWORD dwTick0)
	{
	return dwTick1 - dwTick0;
	}

inline string GetLastErrorText()
	{
	return GetErrorText(GetLastError());
	}

#ifdef SERLOG_MCD
inline BOOL IsValidByte0(BYTE b) {return (b & 0xc0) == 0x40;}
inline BOOL IsValidByte2(BYTE b) {return (b & 0xc0) == 0xc0;}
#endif
