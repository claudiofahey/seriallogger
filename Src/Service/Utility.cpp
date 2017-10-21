/****************************************************************************
 Utility.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
****************************************************************************/

#include "SerialLogger.h"

/*****************************************************************************
 Generic utility functions
*****************************************************************************/

string DCBToString(DCB &dcb)
	{
	stringstream ss;
	char *szDtrControl = 0, *szRtsControl = 0, *szParity = 0, *szStopBits = 0;

	switch (dcb.fDtrControl)
		{
		case DTR_CONTROL_DISABLE:	szDtrControl = "disabled"; break;
		case DTR_CONTROL_ENABLE:	szDtrControl = "enabled"; break;
		case DTR_CONTROL_HANDSHAKE:	szDtrControl = "handshake"; break;
		default:                    szDtrControl = "unknown"; break;
		}

	switch (dcb.fRtsControl)
		{
		case RTS_CONTROL_DISABLE:	szRtsControl = "disabled"; break;
		case RTS_CONTROL_ENABLE:	szRtsControl = "enabled"; break;
		case RTS_CONTROL_HANDSHAKE:	szRtsControl = "handshake"; break;
		case RTS_CONTROL_TOGGLE:	szRtsControl = "toggle"; break;
		default:					szRtsControl = "unknown"; break;
		}

	switch (dcb.Parity)
		{
		case NOPARITY:		szParity = "None"; break;
		case ODDPARITY:		szParity = "Odd"; break;
		case EVENPARITY:	szParity = "Even"; break;
		case MARKPARITY:	szParity = "Mark"; break;
		case SPACEPARITY:	szParity = "Space"; break;
		default:			szParity = "unknown"; break;
		}
	
	switch (dcb.StopBits)
		{
		case ONESTOPBIT:	szStopBits = "1"; break;
		case ONE5STOPBITS:	szStopBits = "1.5"; break;
		case TWOSTOPBITS:	szStopBits = "2"; break;
		default:            szStopBits = "unknown"; break;
		}

	ss << "DCBlength: " << dcb.DCBlength << "\n";
	ss << "BaudRate: " << dcb.BaudRate << "\n";
	ss << "fBinary: " << dcb.fBinary << "\n";
	ss << "fParity: " << dcb.fParity << "\n";
	ss << "fOutxCtsFlow: " << dcb.fOutxCtsFlow << "\n";
	ss << "fOutxDsrFlow: " << dcb.fOutxDsrFlow << "\n";
	ss << "fDtrControl: " << szDtrControl << "\n";
	ss << "fDsrSensitivity: " << dcb.fDsrSensitivity << "\n";
	ss << "fTXContinueOnXoff: " << dcb.fTXContinueOnXoff << "\n";
	ss << "fOutX: " << dcb.fOutX << "\n";
	ss << "fInX: " << dcb.fInX << "\n";
	ss << "fErrorChar: " << dcb.fErrorChar << "\n";
	ss << "fNull: " << dcb.fNull << "\n";
	ss << "fRtsControl: " << szRtsControl << "\n";
	ss << "fAbortOnError: " << dcb.fAbortOnError << "\n";
	ss << "XonLim: " << dcb.XonLim << "\n";
	ss << "XoffLim: " << dcb.XoffLim << "\n";
	ss << "ByteSize: " << (unsigned) dcb.ByteSize << "\n";
	ss << "Parity: " << szParity << "\n";
	ss << "StopBits: " << szStopBits << "\n";
	ss << "XonChar: " << (unsigned) dcb.XonChar << "\n";
	ss << "XoffChar: " << (unsigned) dcb.XoffChar << "\n";
	ss << "ErrorChar: " << (unsigned) dcb.ErrorChar << "\n";
	ss << "EofChar: " << (unsigned) dcb.EofChar << "\n";
	ss << "EvtChar: " << (unsigned) dcb.EvtChar << "\n";

	return ss.str();
	}

LPTSTR GetErrorText(LPTSTR lpszBuf, DWORD dwSize, DWORD dwErr)
	{
    DWORD dwRet;
    LPTSTR lpszTemp = NULL;

    dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, dwErr, LANG_NEUTRAL, (LPTSTR) &lpszTemp, 0, NULL);

    // supplied buffer is not long enough
    if (!dwRet || ((long)dwSize < (long)dwRet+14))
        lpszBuf[0] = TEXT('\0');
    else
		{
        lpszTemp[lstrlen(lpszTemp)-2] = TEXT('\0');  //remove cr and newline character
        sprintf(lpszBuf, TEXT("%s (%u)"), lpszTemp, dwErr);
		}

    if (lpszTemp)
        LocalFree((HLOCAL) lpszTemp );

    return lpszBuf;
	}

string GetErrorText(DWORD dwErr)
	{
    DWORD dwRet;
    LPTSTR lpszTemp = NULL;
	stringstream ss;

    dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, dwErr, LANG_NEUTRAL, (LPTSTR) &lpszTemp, 0, NULL);

    if (dwRet)
		{
        lpszTemp[lstrlen(lpszTemp)-2] = 0;  //remove cr and newline character
		ss << lpszTemp << " (" << dwErr << ')';
		}
	else
		ss << "Unknown error message (" << dwErr << ')';

    if (lpszTemp)
        LocalFree(lpszTemp);

	return ss.str();
	}

BOOL CloseAndNullHandle(HANDLE &h)
	{
	if (h == NULL)
		return TRUE;
	if (h == INVALID_HANDLE_VALUE || CloseHandle(h))
		{
		h = NULL;
		return TRUE;
		}
	else
		return FALSE;
	}

/* memchr: Like memchr but searches for first occurance of c1 or c2 */
void* __cdecl memchr(const void *buf, int chr1, int chr2, size_t cnt)
	{
    while (cnt && (*(unsigned char *) buf != (unsigned char) chr1)
			   && (*(unsigned char *) buf != (unsigned char) chr2))
		{
        buf = (unsigned char *) buf + 1;
        cnt--;
        }
    return cnt ? (void *) buf : NULL;
	}

/* memrchr: Like above but search for last occurance of c1 or c2 */
void* __cdecl memrchr(const void *buf, int chr1, int chr2, size_t cnt)
	{
	buf = (unsigned char*) buf + cnt - 1;
    while (cnt && (*(unsigned char *) buf != (unsigned char) chr1)
			   && (*(unsigned char *) buf != (unsigned char) chr2))
		{
        buf = (unsigned char *) buf - 1;
        cnt--;
        }
    return cnt ? (void *) buf : NULL;
	}

//#ifdef SERLOG_MCD

/* outputs a bstring (byte string) in hex text format */
std::ostream& operator<<(std::ostream& os, bstring& bstr)
	{
	for (bstring::const_iterator i = bstr.begin() ; i < bstr.end() ; ++i)
		{
		if (i != bstr.begin())
			os << " ";
		os.width(2);
		os.fill('0');
		os << std::hex;
		os << (int) *i;
		}
	return os;
	}

/****************************************************************************
 CSqlTimestamp Implementation
****************************************************************************/

CSqlTimestamp::CSqlTimestamp()
	{
	Clear();
	}

void CSqlTimestamp::Clear()
	{
	ZeroMemory(&ts, sizeof(ts));
	}

void CSqlTimestamp::Set(SYSTEMTIME& st)
	{
	ts.year = st.wYear;
	ts.month = st.wMonth;
	ts.day = st.wDay;
	ts.hour = st.wHour;
	ts.minute = st.wMinute;
	ts.second = st.wSecond;
	ts.fraction = st.wMilliseconds * 1000000;
	}

//#endif	// SERLOG_MCD

/****************************************************************************
 CUpdatableString Implementation
****************************************************************************/

CUpdatableString::CUpdatableString(string str):
	string(str),
	szBuf(0),
	nBufLength(0)
	{
	}

CUpdatableString::CUpdatableString(char *sz):
	string(sz),
	szBuf(0),
	nBufLength(0)
	{
	}

CUpdatableString::~CUpdatableString()
	{
	delete [] szBuf;
	}

char *CUpdatableString::GetBuffer(unsigned long nMinBufLength)
	{
	if (nMinBufLength == 0)
		{
		nMinBufLength = size() + 1;
		}
	if (nBufLength < nMinBufLength)
		{
		if (szBuf)
			delete [] szBuf;
		szBuf = new char[nMinBufLength];
		nBufLength = nMinBufLength;
		}
	unsigned long nCopySize = (size() < nMinBufLength - 1) ? size() : nMinBufLength - 1;
	copy(szBuf, nCopySize);
	szBuf[nCopySize] = 0;
	//PrintInfoToLog("CUpdatableString::GetBuffer: szBuf(%d,%d)=[%s]", nCopySize, nMinBufLength, szBuf);
	return szBuf;
	}

CUpdatableString &CUpdatableString::ReleaseBuffer()
	{
	assign(szBuf);
	return *this;
	}
