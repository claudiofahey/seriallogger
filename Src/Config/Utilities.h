/*****************************************************************************
 Utilities.h
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

CString GetErrorText(DWORD dwError);

CString GetAppFilename();
CString GetDirectoryFromPath(CString &path);
CString GetFilenameFromPath(CString &path);
CString GetAppDirectory();

LONG RegDeleteKeyTree(HKEY hkey, LPCTSTR szSubKey);
LONG RegDeleteKeyIfEmpty(HKEY hkey, LPCTSTR szSubKey);

inline CString GetLastErrorText()
	{
	return GetErrorText(GetLastError());
	}
