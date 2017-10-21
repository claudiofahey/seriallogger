# Microsoft Developer Studio Project File - Name="SMConfig" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SMConfig - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SerialLoggerConfig.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SerialLoggerConfig.mak" CFG="SMConfig - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SMConfig - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SMConfig - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SMConfig - Win32 Release with Symbols" (based on "Win32 (x86) Application")
!MESSAGE "SMConfig - Win32 MCD Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SMConfig - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /machine:I386 /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /libpath:"D:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /force

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Release with Symbols"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SMConfig___Win32_Release_with_Symbols"
# PROP BASE Intermediate_Dir "SMConfig___Win32_Release_with_Symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SymbolRelease"
# PROP Intermediate_Dir "SymbolRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Z7 /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /debug /debugtype:coff /machine:I386 /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /debug /debugtype:both /machine:I386 /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SMConfig - Win32 MCD Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SMConfig___Win32_MCD_Release"
# PROP BASE Intermediate_Dir "SMConfig___Win32_MCD_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SMConfig___Win32_MCD_Release"
# PROP Intermediate_Dir "SMConfig___Win32_MCD_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "SERLOG_MCD" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /machine:I386 /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /machine:I386 /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "SMConfig - Win32 Release"
# Name "SMConfig - Win32 Debug"
# Name "SMConfig - Win32 Release with Symbols"
# Name "SMConfig - Win32 MCD Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\BuildTimestamp.cpp
# End Source File
# Begin Source File

SOURCE=.\CATSETS.CPP
# End Source File
# Begin Source File

SOURCE=.\EmailPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceListDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OdbcPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\RegCode\RegCodeCommon.cpp

!IF  "$(CFG)" == "SMConfig - Win32 Release"

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Debug"

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Release with Symbols"

!ELSEIF  "$(CFG)" == "SMConfig - Win32 MCD Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RegDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SerialLoggerCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Smtp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeoutPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Base64.h
# End Source File
# Begin Source File

SOURCE=..\Common\BuildTimestamp.h
# End Source File
# Begin Source File

SOURCE=.\CATSETS.H
# End Source File
# Begin Source File

SOURCE=..\Common\CommonResource.h
# End Source File
# Begin Source File

SOURCE=.\EmailPage.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\InstanceListDoc.h
# End Source File
# Begin Source File

SOURCE=.\InstanceListView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OdbcPage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\RegCode\RegCodeCommon.h
# End Source File
# Begin Source File

SOURCE=.\RegDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Common\SerialLoggerCommon.h
# End Source File
# Begin Source File

SOURCE=.\SMConfig.h
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Smtp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TableDialog.h
# End Source File
# Begin Source File

SOURCE=.\TimeoutPage.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\SMConfig.rc
# End Source File
# Begin Source File

SOURCE=.\res\SMConfig.rc2
# End Source File
# End Group
# End Target
# End Project
