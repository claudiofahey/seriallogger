/*****************************************************************************
 Utilities.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "stdafx.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString GetAppFilename()
	{
	CString s;
	DWORD rc = GetModuleFileName(NULL, s.GetBuffer(512), 512);
	s.ReleaseBuffer();
	if (rc == 0)
		s = "";
	return s;
	}

CString GetDirectoryFromPath(CString &path)
	{
	CString dir;
	int i = path.ReverseFind('\\');
	if (i < 0)
		dir = "";
	else
		dir = path.Left(i);
	return dir;
	}

CString GetFilenameFromPath(CString &path)
	{
	CString file;
	int i = path.ReverseFind('\\');
	if (i < 0)
		file = path;
	else
		file = path.Mid(i + 1);
	return file;
	}

CString GetAppDirectory()
	{
	return GetDirectoryFromPath(GetAppFilename());
	}

CString GetErrorText(DWORD dwError)
	{
	CString s;
    DWORD dwRc;
	DWORD bufsize = 256 + 15;

    dwRc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL, dwError, LANG_NEUTRAL, s.GetBuffer(bufsize), bufsize, NULL);

	if (dwRc == 0)
		s.Format("Error %u", dwError);
	else
		{
		s.ReleaseBuffer();
		CString s2;
		s2.Format(" (%u)", dwError);
		s += s2;
		}

    return s;
	}

void _Assert(char *exp, char *file, unsigned line)
	{
#ifdef _DEBUG
	if (AfxAssertFailedLine(file, line))
		AfxDebugBreak();
#endif
	} 

LONG RegDeleteKeyTree(HKEY hkey, LPCTSTR szSubKey)
	{
	LONG lRc = 0;
	HKEY hkeySub = 0;
	DWORD dwIndex = 0;
	const unsigned cbNameMax = 128;
	char szKeyName[cbNameMax];
	FILETIME ftLastWriteTime = {0};

	// open subkey
	lRc = RegOpenKeyEx(hkey, szSubKey, 0, KEY_ALL_ACCESS, &hkeySub);
	if (lRc != ERROR_SUCCESS)
		return lRc;

	// enumerate subkey and recurse into each subkey's subkey
	for (dwIndex = 0 ;; dwIndex++)
		{
		DWORD cbName = cbNameMax;
		lRc = RegEnumKeyEx(hkeySub, dwIndex, szKeyName, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
		if (lRc == ERROR_NO_MORE_ITEMS)
			break;
		if (lRc != ERROR_SUCCESS)
			return lRc;

		// recurse into each subkey's subkey
		lRc = RegDeleteKeyTree(hkeySub, szKeyName);
		if (lRc != ERROR_SUCCESS)
			return lRc;
		}

	// close subkey
	lRc = RegCloseKey(hkeySub);
	hkeySub = 0;

	// delete subkey
	lRc = RegDeleteKey(hkey, szSubKey);
	return lRc;
	}

LONG RegDeleteKeyIfEmpty(HKEY hkey, LPCTSTR szSubKey)
	{
	LONG lRc = 0;
	HKEY hkeySub = 0;
	const unsigned cbNameMax = 128;
	char szBuf[cbNameMax];
	FILETIME ftLastWriteTime = {0};
	DWORD cbName = 0;

	// open subkey
	lRc = RegOpenKeyEx(hkey, szSubKey, 0, KEY_ALL_ACCESS, &hkeySub);
	if (lRc != ERROR_SUCCESS)
		return lRc;

	// determine if there are any keys
	cbName = cbNameMax;
	lRc = RegEnumKeyEx(hkeySub, 0, szBuf, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
	if (lRc == ERROR_SUCCESS)
		return lRc;		// at least one key exists; don't delete subkey
	if (lRc != ERROR_NO_MORE_ITEMS)
		return lRc;		// error occured
	// else no subkeys exists

	// determine if there are any values
	cbName = cbNameMax;
	lRc = RegEnumValue(hkeySub, 0, szBuf, &cbName, NULL, NULL, NULL, NULL);
	if (lRc == ERROR_SUCCESS)
		return lRc;		// at least one value exists; don't delete subkey
	if (lRc != ERROR_NO_MORE_ITEMS)
		return lRc;		// error occured
	// else no values exists

	// close subkey
	lRc = RegCloseKey(hkeySub);
	hkeySub = 0;

	// delete subkey
	lRc = RegDeleteKey(hkey, szSubKey);
	return lRc;	
	}
