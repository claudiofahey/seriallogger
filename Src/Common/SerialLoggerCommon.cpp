/*****************************************************************************
 SerialLoggerCommon.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "SerialLoggerCommon.h"

#ifdef TRACEHACK
 void __cdecl AfxTrace(LPCTSTR lpszFormat, ...);
 #define TRACE              ::AfxTrace
#else
 #define TRACE				(void)
#endif

#ifndef CONFIGAPP
 BOOL VerboseLevel(DWORD dw);
 void PrintToLog(char *fmt, ...);
 void PrintInfoToLog(char *fmt, ...);
#endif

/*****************************************************************************
 CCommonParameters Implementation
*****************************************************************************/

CCommonParameters::CCommonParameters():
	strRegName(""),
	dwRegCode1(0),
	dwRegCodeA(0),
	dwRegCodeB(0),
	dwRegistryVersion(0),
	// calculated variables
	flRegistered(0)
	{
	}

BOOL CCommonParameters::LoadParameters()
	{
	HKEY hkeyCommon = 0;
	LONG lRc = 0;
	BOOL flRc = 0;

	string strKeyNameCommon = SZREGKEYCOMMON;

	lRc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyNameCommon.c_str(), 0, 
		KEY_QUERY_VALUE, &hkeyCommon);
	if (lRc != ERROR_SUCCESS)
		return FALSE;

	flRc = RegQueryString(hkeyCommon, "RegName", strRegName);
	flRc = RegQueryInt(hkeyCommon, "RegCode1", dwRegCode1);
	flRc = RegQueryInt(hkeyCommon, "RegCodeA", dwRegCodeA);
	flRc = RegQueryInt(hkeyCommon, "RegCodeB", dwRegCodeB);
	flRc = RegQueryInt(hkeyCommon, "RegistryVersion", dwRegistryVersion);	// not used

	RegCloseKey(hkeyCommon);
	hkeyCommon = 0;

	return TRUE;
	}

BOOL CCommonParameters::LoadDownlevelParameters()
	{
	HKEY hkeyCommon = 0;
	LONG lRc = 0;
	BOOL flRc = 0;

	string strKeyNameCommon = "SYSTEM\\CurrentControlSet\\Services\\" SZSERVICENAME "\\Parameters";

	lRc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyNameCommon.c_str(), 0, 
		KEY_QUERY_VALUE, &hkeyCommon);
	if (lRc != ERROR_SUCCESS)
		return FALSE;

	flRc = RegQueryString(hkeyCommon, "RegName", strRegName);
	flRc = RegQueryInt(hkeyCommon, "RegCode1", dwRegCode1);

	RegCloseKey(hkeyCommon);
	hkeyCommon = 0;

	return TRUE;
	}

void CCommonParameters::CalcParameters()
	{
	// Validate registration code
	int iRc = ValidateRegCode(strRegName.c_str(), dwRegCode1);
	//TRACE("CCommonParameters::CalcParameters: ValidateRegCode(%s,%x)=%d\n", 
	//	strRegName.c_str(), dwRegCode1, iRc);
	if (iRc)
		flRegistered = 1;
	else
		{
		dwRegCode1 = 0;
		strRegName = SZUNLICENSEDTEXT;
		}
	}

/*****************************************************************************
 CInstanceParameters Implementation
*****************************************************************************/

CInstanceParameters::CInstanceParameters():
	nInstance(0),
	strServiceName(""),
	strServiceDisplayName(""),
	strLogFilename(""),
	dwTimestampFormat(0),
	dwLogRotation(0),			// validated in LoadParameters
	strSmtpServer(""),
	strSmtpFromAddress(""),
	strSmtpToAddress(""),
	dwPauseTimeout(INFINITE),
	dwVerboseLevel(VERBOSE_STARTSTOP | VERBOSE_CONFIGURATION),
	flStartAutomatically(0),
	//dwMaxLineLength(1024),
	strSQLConnectionString(""),
	strSQLWriteLineStatement(""),
	dwMaxProcessedLineLength(1024),
	// calculated variables:
	strLogFilenamePrefix(""),
	strLogFilenameSuffix(""),
	flSmtp(0),
	flLogFile(0),
	flOdbc(0),
	// GUI-only variables
	flJustStarted(0),
	flRestart(0)
	{
	ssStatus.dwCurrentState = 0;
	}

// Note: strServiceName must be set before this is called
BOOL CInstanceParameters::LoadParameters()
	{
	HKEY hkeyParam = 0;
	LONG lRc = 0;
	BOOL flRc = 0;
	DWORD dwIndex = 0;
	const unsigned cbNameMax = 128;
	DWORD cbName = 0;
	unsigned nInstance = 0;
	FILETIME ftLastWriteTime = {0};

	string strKeyNameParameters = string("SYSTEM\\CurrentControlSet\\Services\\") + strServiceName +
		"\\Parameters";

	//
	// Open Parameters key
	//
			
	HKEY hkeyInstance = 0;
	lRc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyNameParameters.c_str(), 0, 
		KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hkeyInstance);
	if (lRc != ERROR_SUCCESS)
		return FALSE;

	//
	// Open Parameters\Port1 key
	//

	BOOL flHasPortKey = TRUE;
	HKEY hkeyPort = 0;
	lRc = RegOpenKeyEx(hkeyInstance, "Port1", 0, 
		KEY_QUERY_VALUE, &hkeyPort);
	if (lRc != ERROR_SUCCESS)
		{
		// Port1 key doesn't exist; port parameters will be looked up in Parameters key
		flHasPortKey = FALSE;
		hkeyPort = hkeyInstance;
		}

	//
	// Read Parameters values
	//

	CInstanceParameters &ip = *this;
	flRc = RegQueryString(hkeyInstance, "LogFilename", ip.strLogFilename);
	flRc = RegQueryInt(hkeyInstance, "TimestampFormat", ip.dwTimestampFormat);

	flRc = RegQueryInt(hkeyInstance, "LogRotation", ip.dwLogRotation);
	if (!flRc || ip.dwLogRotation < LOGROTATION_MIN || ip.dwLogRotation > LOGROTATION_MAX)
		ip.dwLogRotation = LOGROTATION_NONE;

	flRc = RegQueryString(hkeyInstance, "SmtpFromAddress", ip.strSmtpFromAddress);
	flRc = RegQueryString(hkeyInstance, "SmtpToAddress", ip.strSmtpToAddress);
	flRc = RegQueryString(hkeyInstance, "SmtpServer", ip.strSmtpServer);
	flRc = RegQueryInt(hkeyInstance, "PauseTimeout", ip.dwPauseTimeout);
	flRc = RegQueryInt(hkeyInstance, "VerboseLevel", ip.dwVerboseLevel);
	flRc = RegQueryString(hkeyInstance, "SQLConnectionString", ip.strSQLConnectionString);
	flRc = RegQueryString(hkeyInstance, "SQLWriteLineStatement", ip.strSQLWriteLineStatement);
	flRc = RegQueryInt(hkeyInstance, "MaxLineLength", ip.dwMaxProcessedLineLength);

	//
	// Read Parameters\Port1 values
	//

	CPortParameters pp;
	flRc = RegQueryString(hkeyPort, "PortName", pp.strPortName);
	flRc = RegQueryInt(hkeyPort, "BaudRate", pp.BaudRate);

	flRc = RegQueryInt(hkeyPort, "ByteSize", pp.ByteSize);
	if (!flRc || pp.ByteSize < 4 || pp.ByteSize > 8)
		pp.ByteSize = 8;		// 8 bits default

	flRc = RegQueryInt(hkeyPort, "Parity", pp.Parity);
	if (!flRc || pp.Parity < 0 || pp.Parity > 4)
		pp.Parity = 0;		// no parity default

	flRc = RegQueryInt(hkeyPort, "StopBits", pp.StopBits);
	if (!flRc || pp.StopBits < 0 || pp.StopBits > 2)
		pp.StopBits = 0;		// no parity default

	flRc = RegQueryInt(hkeyPort, "ComBufferSize", pp.dwComBufferSize);

	flRc = RegQueryInt(hkeyPort, "Timeout", pp.dwTimeout);
	flRc = RegQueryInt(hkeyPort, "WarningInterval", pp.dwWarningInterval);

	flRc = RegQueryInt(hkeyPort, "DayOfWeekMask", pp.dwDayOfWeekMask);
	pp.dwDayOfWeekMask &= 0x7f;

	flRc = RegQueryInt(hkeyPort, "StartHour", pp.dwStartHour);
	if (flRc)
		flRc = RegQueryInt(hkeyPort, "StartMin", pp.dwStartMin);

	flRc = RegQueryInt(hkeyPort, "EndHour", pp.dwEndHour);
	if (flRc)
		flRc = RegQueryInt(hkeyPort, "EndMin", pp.dwEndMin);

	flRc = RegQueryString(hkeyPort, "NetSendUsers", pp.strNetSendUsers);
	flRc = RegQueryString(hkeyPort, "CustomTimeoutCmd", pp.strTimeoutCmd);

	//flRc = RegQueryInt(hkeyPort, "VerboseLevel", pp.dwVerboseLevel);

	ip.listPp.push_back(pp);

	if (flHasPortKey && hkeyPort)
		{
		RegCloseKey(hkeyPort);
		hkeyPort = 0;
		}

	if (hkeyInstance)
		{
		RegCloseKey(hkeyInstance);
		hkeyInstance = 0;
		}

	return TRUE;
	}

void CInstanceParameters::CalcParameters()
	{
	listPp[0].CalcParameters();

	ParseLogFilename();

	//
	// Parse comma-separated list of SMTP to addresses
	//
	listSmtpToAddress.clear();
	StringListFromString(listSmtpToAddress, strSmtpToAddress);

	// determine main service modes
	flOdbc = (!strSQLConnectionString.empty() && !strSQLWriteLineStatement.empty());
	flLogFile = !strLogFilename.empty();
	flSmtp = (flLogFile && !strSmtpFromAddress.empty() && listSmtpToAddress.size() > 0 && 
			 !strSmtpServer.empty()) ? TRUE : FALSE;
	
#if 0
	// determine max line length (not including timestamp prefix)
	dwMaxLineLength = dwMaxProcessedLineLength;
	switch (dwTimestampFormat)
		{
		case 1:
			dwMaxLineLength -= 25;
		}
#endif
	}

//
// Break log filename into prefix and suffix
//
void CInstanceParameters::ParseLogFilename()
	{
	string::size_type pos2(0);

	pos2 = strLogFilename.find_last_of(".\\/:");
	if (pos2 != string::npos && strLogFilename[pos2] != '.')
		pos2 = string::npos;
	strLogFilenamePrefix = strLogFilename.substr(0, pos2);
	strLogFilenameSuffix = (pos2 == string::npos) ? "" : strLogFilename.substr(pos2, string::npos);
	}

void CInstanceParameters::SetServiceInstance(unsigned nInstance)
	{
	this->nInstance = nInstance;
	strServiceName = GetServiceName(nInstance);
	strServiceDisplayName = GetServiceDisplayName(nInstance);
	}

/*****************************************************************************
 CPortParameters Implementation
*****************************************************************************/

CPortParameters::CPortParameters():
	strPortName(""),
	BaudRate(2400),
	ByteSize(8),				// validated in LoadParameters
	Parity(0),					// validated in LoadParameters
	StopBits(0),				// validated in LoadParameters
	dwComBufferSize(0),
	strNetSendUsers(""),
	strTimeoutCmd(""),
	dwTimeout(0),
	dwWarningInterval(600),
	dwDayOfWeekMask(0x3e),		// default: Mon through Fri
	dwStartHour(9),
	dwStartMin(0),
	dwEndHour(18),
	dwEndMin(0),
	// calculated variables:
	fStartHour(0),
	fEndHour(0)
	{
	}

void CPortParameters::CalcParameters()
	{
	//
	// Parse comma-separated list of net send users and build list
	//
	listNetSendUsers.clear();
	StringListFromString(listNetSendUsers, strNetSendUsers);

	// Convert timeout times to doubles
	fStartHour = (float) dwStartHour + (dwStartMin/60.0);
	fEndHour = (float) dwEndHour + (dwEndMin/60.0);
	}

/*****************************************************************************
 ci_char_traits Implementation
*****************************************************************************/

bool ci_char_traits::eq(const char& c1, const char& c2)
	{
	return toupper(c1) == toupper(c2);
	};

bool ci_char_traits::lt(const char& c1, const char& c2)
	{ 
	return toupper(c1) < toupper(c2); 
	};

int ci_char_traits::compare(const char* s1, const char* s2, size_t n)
	{
    for (size_t i = 0; i < n; ++i)
		{
		if (!eq(s1[i], s2[i]))
			return lt(s1[i], s2[i]) ? -1 : 1;
        }
     return 0;
	}

const char* ci_char_traits::find(const char* s, size_t n, const char& c)
	{
    for (size_t i = 0; i < n; ++i)
		{
        if (eq(s[i], c))
			return &(s[i]);
        }
    return 0;
	}

/*****************************************************************************
 Misc. functions
*****************************************************************************/

string GetServiceName(unsigned nInstance)
	{
	stringstream ssServiceName;
	ssServiceName << SZSERVICENAME;
	ssServiceName << nInstance;
	return ssServiceName.str();
	}

string GetServiceDisplayName(unsigned nInstance)
	{
	stringstream ssDisplayName;
	ssDisplayName << SZSERVICEDISPLAYNAME " (";
	ssDisplayName << nInstance;
	ssDisplayName << ")";			
	return ssDisplayName.str();
	}

BOOL CalcCurrentLogFileRotation(const CInstanceParameters &ip, string &strLogFilename,
		LONGLONG &llRotateRangeBegin, LONGLONG &llRotateRangeEnd)
	{
	if (ip.dwLogRotation == LOGROTATION_NONE)
		{
		llRotateRangeBegin = 0;
		llRotateRangeEnd = 0x7fffffffffffffff;

		strLogFilename = ip.strLogFilename;
		}
	else
		{		
		SYSTEMTIME st1 = {0};		// rotate range begin
		//SYSTEMTIME st2 = {0};		// rotate range end
		LONGLONG llSpan = 0;
		stringstream ss("");

		ss << ip.strLogFilenamePrefix;
		ss.fill('0');

		GetLocalTime(&st1);

		llRotateRangeBegin = 0;
		llRotateRangeEnd = 0;

		//
		// Determine rotate range begin time and span
		//
		st1.wMilliseconds = 0;
		st1.wSecond = 0;
		switch (ip.dwLogRotation)
			{
			case LOGROTATION_MONTHLY:
				{
				st1.wMinute = 0;
				st1.wHour = 0;
				st1.wDay = 1;

				//
				// To determine the span, we must determine the number of days
				// in the month.
				//
				SYSTEMTIME stNextMonth = st1;
				stNextMonth.wMonth++;
				if (stNextMonth.wMonth > 12)	// carry month to year if necessary
					{
					stNextMonth.wMonth = 1;
					stNextMonth.wYear++;
					}

				// calc rotate range end here instead of after switch
				llRotateRangeEnd = SystemTimeToQuad(stNextMonth);

				// calc log filename
				ss << st1.wYear;
				ss.width(2); ss << st1.wMonth;
				break;
				}

			case LOGROTATION_WEEKLY:
				{
				// Weekly logs are rotated on Sundays at 0:00
				st1.wMinute = 0;
				st1.wHour = 0;

				// start with today at 0:00
				llRotateRangeBegin = SystemTimeToQuad(st1);

				// subtract number of days past last Sunday to get the previous Sunday at 0:00.
				llRotateRangeBegin -= (LONGLONG) st1.wDayOfWeek * 24 * 60 * 60 * 1000 * 1000 * 10;

				// convert back to system time structure to get year/month/day
				st1 = QuadToSystemTime(llRotateRangeBegin);

				llSpan = (LONGLONG) 7 * 24 * 60 * 60 * 1000 * 1000 * 10;	// 7 day span

				ss << st1.wYear;
				ss.width(2); ss << st1.wMonth;
				ss.width(2); ss << st1.wDay;
				break;
				}

			case LOGROTATION_DAILY:
				st1.wMinute = 0;
				st1.wHour = 0;
				llSpan = (LONGLONG) 24 * 60 * 60 * 1000 * 1000 * 10;

				ss << st1.wYear;
				ss.width(2); ss << st1.wMonth;
				ss.width(2); ss << st1.wDay;
				break;

			case LOGROTATION_HOURLY:
				st1.wMinute = 0;
				llSpan = (LONGLONG) 60 * 60 * 1000 * 1000 * 10;

				ss << st1.wYear;
				ss.width(2); ss << st1.wMonth;
				ss.width(2); ss << st1.wDay;
				ss.width(2); ss << st1.wHour;
				break;

			case LOGROTATION_MINUTELY:
				llSpan = (LONGLONG) 60 * 1000 * 1000 * 10;

				ss << st1.wYear;
				ss.width(2); ss << st1.wMonth;
				ss.width(2); ss << st1.wDay;
				ss.width(2); ss << st1.wHour;
				ss.width(2); ss << st1.wMinute;
				break;
			}

		// Calc rotate range begin/end only if not calculated above
		if (!llRotateRangeBegin)
			llRotateRangeBegin = SystemTimeToQuad(st1);
		if (!llRotateRangeEnd)
			llRotateRangeEnd = llRotateRangeBegin + llSpan;

		ss << ip.strLogFilenameSuffix;
		strLogFilename = ss.str();
		}

	return TRUE;
	}

// Prepares CMailThreadInfo given CInstanceParameters and mti.strLogFilename
BOOL PrepareMailThreadInfo(CMailThreadInfo &mti, CInstanceParameters &ip)
	{
	CSmtpAddress addr;
	CSmtpMessageBody body;
	CSmtpAttachment attach;

	mti.msg.Sender.Address = ip.strSmtpFromAddress.c_str();

	stringlist::iterator i;		
	for (i = ip.listSmtpToAddress.begin() ; i != ip.listSmtpToAddress.end() ; i++)
		{
		addr.Address = *i;
		mti.msg.Recipients.Add(addr);
		}
	
	mti.msg.Subject = SZSERVICEDISPLAYNAME " Log File ";
	string fn = GetFilenameFromPath(mti.strLogFilename);
	mti.msg.Subject += fn.c_str();

	body = string("Attached is the log file ") + fn + ".";
	mti.msg.Message.Add(body);

	attach = mti.strLogFilename;
	// Force Base 64 encoding to avoid bug with quoted printable encoding
	attach.TransferEncoding = encodeBase64;
	mti.msg.Attachments.Add(attach);

	//mti.strLogFilename = strLogFilename;
	mti.strSmtpServer = ip.strSmtpServer;

	return TRUE;
	}

int StringListFromString(stringlist &sl, const string &s)
	{
	//
	// Parse comma-separated list of net send users and build list
	//
	string tok("");
	string::size_type pos1(0), pos2(0);
	for (pos1 = 0 ;;)
		{
		pos2 = s.find(',', pos1);
		tok = stringtrim(s.substr(pos1, pos2 - pos1), " \t\n");

		if (!tok.empty())
			sl.insert(sl.end(), tok);

		if (pos2 == s.npos)
			break;

		pos1 = pos2 + 1;
		}
	return sl.size();
	}

string QuadTimeToString(LONGLONG &ll)
	{
	stringstream ss("");
	SYSTEMTIME st = QuadToSystemTime(ll);

	ss.fill('0');
	ss.width(2); ss << st.wMonth << '/';
	ss.width(2); ss << st.wDay << '/';
	ss << st.wYear << ' ';
	ss.width(2); ss << st.wHour << ':';
	ss.width(2); ss << st.wMinute << ':';
	ss.width(2); ss << st.wSecond << '.';
	ss.width(3); ss << st.wMilliseconds << '.';

	return ss.str();
	}

string stringtrim(string s, string sTrimChars)
	{
	string::size_type pos1 = s.find_first_not_of(sTrimChars);
	if (pos1 == string::npos)
		return string("");
	string::size_type pos2 = s.find_last_not_of(sTrimChars);
	return s.substr(pos1, pos2 - pos1 + 1);
	}

string GetFilenameFromPath(string &path)
	{
	string::size_type pos1 = path.find_last_of("\\/:");
	if (pos1 == string::npos)
		return path;
	else
		return path.substr(pos1 + 1);
	}

BOOL RegQueryString(HKEY hkey, LPCTSTR szValueName, string &s)
	{
	int iBufSize = 128;		// initial size of buffer
	LPTSTR pszBuf = 0;
	LONG lRc = 0;
	DWORD dwType = 0, cb = 0;
	BOOL flRetVal = FALSE;

	if (!hkey)
		return FALSE;

	for (;;)
		{
		assert(pszBuf == 0);
		pszBuf = new CHAR[iBufSize];
		assert(pszBuf);

		cb = iBufSize;
		lRc = RegQueryValueEx(hkey, szValueName, NULL, &dwType, (LPBYTE) pszBuf, &cb);
		if (lRc == ERROR_SUCCESS)
			{
			if (dwType == REG_SZ || dwType == REG_EXPAND_SZ)
				{
				s = pszBuf;
				flRetVal = TRUE;
				}
			break;
			}
		else if (lRc == ERROR_MORE_DATA)
			{
			delete [] pszBuf;
			pszBuf = 0;
			iBufSize = ((int) cb >= 2*iBufSize) ? cb : 2*iBufSize;
			}
		else
			break;
		}

	delete [] pszBuf;
	assert(pszBuf);
	pszBuf = 0;

	return flRetVal;
	}

BOOL RegQueryInt(HKEY hkey, LPCTSTR szValueName, DWORD &dw)
	{
	LONG lRc = 0;
	DWORD dwType = 0, cb = 0, dwBuf = 0;
	
	if (!hkey)
		return FALSE;

	cb = sizeof(DWORD);
	lRc = RegQueryValueEx(hkey, szValueName, NULL, &dwType, (LPBYTE) &dwBuf, &cb);
	if (lRc == ERROR_SUCCESS && dwType == REG_DWORD)
		{
		dw = dwBuf;
		return TRUE;
		}
	else
		return FALSE;
	}

/*****************************************************************************
 ODBC functions
*****************************************************************************/

SQLRETURN CheckSQLError1(SQLSMALLINT HandleType, SQLHANDLE h, SQLRETURN rc1, const char* szSourceFile,
  						 unsigned SourceLine, unsigned flThrowExceptionOnError)
	{
	#if defined(_DEBUG) && !defined(CONFIGAPP)		// only use for service debug mode
	if (VerboseLevel(VERBOSE_DEBUG) &&
		(rc1 == SQL_ERROR || rc1 == SQL_INVALID_HANDLE || rc1 == SQL_SUCCESS_WITH_INFO))
		{
		if (rc1 == SQL_ERROR)
			{
			PrintToLog("CheckSQLError1: Error: %s:%d: Return Code %d: %s", szSourceFile, SourceLine,
				rc1, GetSQLErrorText(HandleType, h).c_str());
			}
		else
			{
			PrintToLog("CheckSQLError1: Info: %s:%d: Return Code %d: %s", szSourceFile, SourceLine,
				rc1, GetSQLErrorText(HandleType, h).c_str());
			}
		}
	#endif

	if (flThrowExceptionOnError && (rc1 == SQL_ERROR || rc1 == SQL_INVALID_HANDLE))
		{
		//PrintInfoToLog("CheckSQLError1: throwing exception");
		throw COdbcException(HandleType, h);
		}
	else
		return rc1;
	}

string GetSQLErrorText(SQLSMALLINT HandleType, SQLHANDLE h, stringlist *plistSqlState)
	{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i, MsgLen;
	SQLRETURN rc2;
	stringstream ssMsg;

	if (h)
		{
		i = 1;
		while ((rc2 = SQLGetDiagRec(HandleType, h, i, SqlState, &NativeError,
			   Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
			{
			if (plistSqlState)
				plistSqlState->push_back(string((char*) SqlState));
			if (!ssMsg.str().empty())
				ssMsg << "\r\n";
			ssMsg << "[State " << SqlState << ", Native Error " << NativeError << "]" << Msg;
			i++;
			}
		}
	else
		{
		ssMsg << "Handle is NULL.";
		}
	return ssMsg.str();
	}

BOOL IsRecoverableSQLError(stringlist *plistSqlState)
	{
	stringlist::const_iterator i;
	for (i = plistSqlState->begin() ; i != plistSqlState->end() ; ++i)
		{
		if (i->compare(0, 2, "08") == 0)
			{
			//PrintInfoToLog("IsRecoverableOdbcError: found state prefix 08");
			return TRUE;
			}
		}
	return FALSE;
	}

// returns connection string with non-empty password replaced with "*****"
string GetSafeConnectionString(string &s1)
	{
	// create a case-insensitive string
	ci_string s(s1.c_str());
	ci_string::size_type p1 = s.npos, p2 = s.npos;

	if (s.compare(0, 4, "PWD=") == 0)
		p1 = 0;				// "PWD=" is at beginning of string
	else if ((p1 = s.find(";PWD=")) != s.npos)
		p1++;				// ";PWD=" is in string

	if (p1 == s.npos)
		return s1;			// "PWD=" not found; return unchanged
	p1 += 4;				// now p1 is position of first char of password
	if (p1 == s.size())
		return s1;			// blank password; return unchanged
	p2 = s.find(";", p1);	// find ";" which ends password
	if (p2 == p1)
		return s1;			// blank password; return unchanged
	string retval = s1.substr(0, p1) + string("*****");
	if (p2 != s.npos)
		retval += s1.substr(p2);	// password is not last item in string; append rest of string
	return retval;
	}

string GetSQLIdentifier(string &s)
	{
	string::size_type n = s.find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"01234567890_");
	if (n == s.npos)
		return s;
	else
		return string("[") + s + string("]");
	}
