/*****************************************************************************
 SerialLoggerCommon.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#pragma warning(disable : 4786)		// disable warning about identifier being too long for debug info

#include <windows.h>
//#include <htmlhelp.h>

#include <sql.h>
#include <sqlext.h>

#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "BuildTimestamp.h"
#include "..\CSmtp\smtp.h"

/*****************************************************************************
 Registration code support is bypassed for MCD version
*****************************************************************************/

#ifdef SERLOG_MCD
 inline void SetRegCodeSeed(unsigned long s) {};
 inline char *NormalizeRegName(const char *szRegName, char *szNormalizedRegName, unsigned buflen)
	{
	strncpy(szNormalizedRegName, szRegName, buflen); 
	szNormalizedRegName[buflen-1] = 0;
	return szNormalizedRegName;
	}
 inline int RegCodeFromName(const char *szRegName, unsigned long &dwRegCode1) {dwRegCode1 = 1; return 1;}
 inline int ValidateRegCode(const char *szRegName, unsigned long dwRegCode1) {return 1;}
#else
 #include "..\..\..\RegCode\RegCodeCommon.h"
#endif

/*****************************************************************************
 Common Serial Logger defines
*****************************************************************************/

#define SZAPPNAME            "SerialLogger"
#define SZSERVICENAME        "SerialLogger"
#define SZREGKEYCOMPANY		 "SOFTWARE\\Mu-Consulting"
#define SZREGKEYCOMMON		 SZREGKEYCOMPANY "\\SerialLogger"
#define SZSERVICEVERSION     "1.21a"
#define SZDEPENDENCIES       "\0\0"
#define SZCOPYRIGHTDATE		 "2002"
#define SZUNLICENSEDTEXT	"UNLICENSED EVALUATION VERSION -- 30 DAY TRIAL"

#ifdef SERLOG_MCD
 #define SZSERVICEDISPLAYNAME	"Serial Logger Service (MCD)"
 #define SZCONFIGAPPNAME		"Serial Logger Service Configuration (MCD)"
 #define REGCODESEED			 0xa020b7fc
#else
 #define SZSERVICEDISPLAYNAME	"Serial Logger Service"
 #define SZCONFIGAPPNAME		"Serial Logger Service Configuration"
 #define REGCODESEED			 31415
#endif

#define MAXRUNTICKS			 (14*24*60*60*1000)		// unregistered version runs for at most this many ms

#define EVALDAYS			 30						// total number of days to evaluate
#define EVAL100NS			 ((ULONGLONG) EVALDAYS * 24 * 60 * 60 * 1000 * 1000 * 10)	// in units of 100 nanoseconds

#define SZCOMPANYNAME		 "Mu-Consulting"
#define SZWEBSITE			 "www.muconsulting.com"
#define SZBANNERLINE2		 SZCOMPANYNAME " (" SZWEBSITE ")"

#define VERBOSE_STARTSTOP			0x00000001
#define VERBOSE_CONFIGURATION		0x00000002
#define VERBOSE_TIMEOUT				0x00000004
#define VERBOSE_SMTPSUMMARY			0x00000008
#define VERBOSE_CONFIGMASK			0x0000000f		// sets bits that are controlled by config app
#define VERBOSE_SMTPDETAIL			0x00010000
#define VERBOSE_DEBUG				0x80000000

#define LOGROTATION_NONE		0
#define LOGROTATION_MONTHLY		1
#define LOGROTATION_WEEKLY		2
#define LOGROTATION_DAILY		3
#define LOGROTATION_HOURLY		4
#define LOGROTATION_MINUTELY	5

#define LOGROTATION_MIN		LOGROTATION_NONE
#define LOGROTATION_MAX		LOGROTATION_MINUTELY

#define MAXNETSENDUSERS			10		// maximum number of users in the net send list
#define MAXSZNETSENDLIST		1024	// max size of comma-separated net send list

#define assert(exp)				(void)( (exp) || (_Assert(#exp, __FILE__, __LINE__), 0) )
#define assert2(exp,dispexp)	(void)( (exp) || (_Assert(#exp, __FILE__, __LINE__, #dispexp, (DWORD) dispexp), 0) )

/*****************************************************************************
 Forward class declarations
*****************************************************************************/

class CCommonParameters;
class CInstanceParameters;
class CPortParameters;
class CMailThreadInfo;

/*****************************************************************************
 Type definitions
*****************************************************************************/

typedef std::string string;
typedef std::basic_string<BYTE> bstring;
typedef std::stringstream stringstream;
typedef std::list<std::string> stringlist;

/* ci_char_traits:
   Case-insensitive string class from
   http://gcc.gnu.org/onlinedocs/libstdc++/21_strings/gotw29a.txt and
   http://groups.google.com/groups?hl=en&lr=&ie=UTF8&oe=UTF8&selm=38e0d12b%241_2%40news2.one.net
*/
struct ci_char_traits: public std::char_traits<char>
	{
	static bool eq(const char& c1, const char& c2);
    static bool lt(const char& c1, const char& c2);
	static int compare(const char* s1, const char* s2, size_t n);
	static const char* find(const char* s, size_t n, const char& c);
	};

typedef std::basic_string<char, ci_char_traits> ci_string;

/*****************************************************************************
 Global function prototypes
*****************************************************************************/

string GetServiceName(unsigned nInstance);
string GetServiceDisplayName(unsigned nInstance);
BOOL CalcCurrentLogFileRotation(const CInstanceParameters &ip, string &strLogFilename,
		LONGLONG &llRotateRangeBegin, LONGLONG &llRotateRangeEnd);
BOOL PrepareMailThreadInfo(CMailThreadInfo &mti, CInstanceParameters &ip);
int StringListFromString(stringlist &sl, const string &s);
string QuadTimeToString(LONGLONG &ll);
string stringtrim(string s, string sTrimChars);
string GetFilenameFromPath(string &path);
BOOL RegQueryString(HKEY hkey, LPCTSTR szValueName, string &s);
BOOL RegQueryInt(HKEY hkey, LPCTSTR szValueName, DWORD &dw);

// Implementations for _Assert are defined separately for service and config app
void _Assert(char *exp, char *file, unsigned line);
void _Assert(char *exp, char *file, unsigned line, char *szdispexp, DWORD dwdispexp);

/*****************************************************************************
 Class definitions
*****************************************************************************/

class CCommonParameters
	{
	public:
	CCommonParameters();
	BOOL LoadParameters();
	BOOL LoadDownlevelParameters();
	void CalcParameters();

	//	
	// parameters read from registry directly
	//
	string strRegName;
	DWORD dwRegCode1;
	DWORD dwRegCodeA;
	DWORD dwRegCodeB;
	DWORD dwRegistryVersion;

	//
	// Derived parameters
	//
	BOOL flRegistered;
	};

class CInstanceParameters
	{
	public:
	CInstanceParameters();
	BOOL LoadParameters();
	void CalcParameters();
	void ParseLogFilename();
	void SetServiceInstance(unsigned nInstance);

	// Instance identification/definition
	unsigned nInstance;
	string strServiceName;
	string strServiceDisplayName;

	//	
	// parameters read from registry directly
	//
	string strLogFilename;			// output log filename
	DWORD dwTimestampFormat;		// 0 = none, 1 = format 1
	DWORD dwLogRotation;			// log rotation scheme
	string strSmtpServer;
	string strSmtpFromAddress;
	string strSmtpToAddress;
	DWORD dwPauseTimeout;
	DWORD dwVerboseLevel;
	std::vector<CPortParameters> listPp;
	BOOL flStartAutomatically;		// stored in service config
	string strSQLConnectionString;
	string strSQLWriteLineStatement;
	DWORD dwMaxProcessedLineLength;	// max number of char per line (including prefix, not including CR+LF)

	//
	// Derived parameters
	//
	string strLogFilenamePrefix;
	string strLogFilenameSuffix;
	stringlist listSmtpToAddress;
	BOOL flSmtp;
	BOOL flLogFile;					// true if we are logging to a normal file
	BOOL flOdbc;					// true if we are logging to an ODBC connection

	//
	// GUI-only variables
	//
	SERVICE_STATUS ssStatus;
	BOOL flJustStarted;
	BOOL flRestart;
	};

class CPortParameters
	{
	public:
	CPortParameters();
	void CalcParameters();

	//	
	// parameters read from registry directly
	//
	string strPortName;			// "COM1", etc.
	DWORD BaudRate;
	DWORD ByteSize;
	DWORD Parity;
	DWORD StopBits;
	DWORD dwComBufferSize;		// size of com port input buffer
	string strNetSendUsers;
	string strTimeoutCmd;
	DWORD dwTimeout;			// seconds for timeout
	DWORD dwWarningInterval;	// seconds between warnings
	DWORD dwDayOfWeekMask;
	DWORD dwStartHour, dwStartMin, dwEndHour, dwEndMin;

	//
	// Derived parameters
	//
	stringlist listNetSendUsers;
	double fStartHour;
	double fEndHour;
	};

class CMailThreadInfo
	{
	public:
	CSmtpMessage msg;
	string strSmtpServer;
	string strLogFilename;
	};

/*****************************************************************************
 Inline functions
*****************************************************************************/

inline DWORD RoundToPage(DWORD dw)
	{
	return ((dw + 4095)/4096) * 4096;
	}

inline LONGLONG FileTimeToQuad(FILETIME &ft)
	{
	LARGE_INTEGER li;
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	return li.QuadPart;
	}

inline FILETIME QuadToFileTime(LONGLONG &ll)
	{
	LARGE_INTEGER li;
	FILETIME ft;
	li.QuadPart = ll;
	ft.dwLowDateTime = li.LowPart;
	ft.dwHighDateTime = li.HighPart;
	return ft;
	}

inline LONGLONG SystemTimeToQuad(SYSTEMTIME &st)
	{
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return FileTimeToQuad(ft);
	}

inline SYSTEMTIME QuadToSystemTime(LONGLONG &ll)
	{
	SYSTEMTIME st;
	FILETIME ft;
	ft = QuadToFileTime(ll);
	FileTimeToSystemTime(&ft, &st);
	return st;
	}

inline LONGLONG GetLocalFileTimeQuad()
	{
	SYSTEMTIME st = {0};
	FILETIME ft = {0};

	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	return FileTimeToQuad(ft);
	}

inline LONGLONG GetSystemFileTimeQuad()
	{
	FILETIME ft = {0};
	GetSystemTimeAsFileTime(&ft);
	return FileTimeToQuad(ft);
	}

inline BOOL RegSetString(HKEY hkey, LPCTSTR szValueName, const string &s)
	{
	return RegSetValueEx(hkey, szValueName, 0, REG_SZ, (CONST BYTE*) s.c_str(), 
		s.size() + 1) == ERROR_SUCCESS;
	}

inline BOOL RegSetInt(HKEY hkey, LPCTSTR szValueName, DWORD dw)
	{
	return RegSetValueEx(hkey, szValueName, 0, REG_DWORD, (CONST BYTE*) &dw, 
		sizeof(DWORD)) == ERROR_SUCCESS;
	}

/*****************************************************************************
 ODBC support
*****************************************************************************/

class COdbcException
	{
	public:
	COdbcException(SQLSMALLINT HandleType, SQLHANDLE h)
		{
		this->HandleType = HandleType;
		this->h = h;
		};		
	SQLSMALLINT HandleType;
	SQLHANDLE h;
	};

SQLRETURN CheckSQLError1(SQLSMALLINT HandleType, SQLHANDLE h, SQLRETURN rc1, const char* szSourceFile,
  						 unsigned SourceLine, unsigned flThrowExceptionOnError);
string GetSQLErrorText(SQLSMALLINT HandleType, SQLHANDLE h, stringlist *plistSqlState = 0);
BOOL IsRecoverableSQLError(stringlist *plistSqlState);
string GetSafeConnectionString(string &strConnectionString);
string GetSQLIdentifier(string &s);

#define	AssertSQLEnv(rc, h)		CheckSQLError1(SQL_HANDLE_ENV, (h), (rc), __FILE__, __LINE__, 1)
#define	CheckSQLEnv(rc, h)		CheckSQLError1(SQL_HANDLE_ENV, (h), (rc), __FILE__, __LINE__, 0)
#define	AssertSQLDBC(rc, h)		CheckSQLError1(SQL_HANDLE_DBC, (h), (rc), __FILE__, __LINE__, 1)
#define	CheckSQLDBC(rc, h)		CheckSQLError1(SQL_HANDLE_DBC, (h), (rc), __FILE__, __LINE__, 0)
#define	AssertSQLStmt(rc, h)	CheckSQLError1(SQL_HANDLE_STMT, (h), (rc), __FILE__, __LINE__, 1)
#define	CheckSQLStmt(rc, h)		CheckSQLError1(SQL_HANDLE_STMT, (h), (rc), __FILE__, __LINE__, 0)
