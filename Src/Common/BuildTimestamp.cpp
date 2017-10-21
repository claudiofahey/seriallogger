/*****************************************************************************
 BuildTimestamp.cpp
 Serial Logger Service
 Copyright (C) 2002 by Mu-Consulting

 The use of this source file is restricted as described in the License.txt
 file that accompanies it.  Any modififications to this file must retain
 this notice.  Failure to do so is a violation of the license.
*****************************************************************************/

#include "BuildTimestamp.h"

static char *szBuildDate = __DATE__;
static char *szBuildTime = __TIME__;
static char *szBuildDateTime = __DATE__ " " __TIME__;

const char *GetBuildDate()
	{
	return szBuildDate;
	}

const char *GetBuildTime()
	{
	return szBuildTime;
	}

const char *GetBuildDateTime()
	{
	return szBuildDateTime;
	}
