# Microsoft Developer Studio Project File - Name="SerialMon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SerialMon - Win32 MCD Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SerialLogger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SerialLogger.mak" CFG="SerialMon - Win32 MCD Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SerialMon - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SerialMon - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "SerialMon - Win32 MCD Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "SerialMon - Win32 MCD Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SerialMon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
PostBuild_Cmds=net stop seriallogger & false	*copy $(TargetPath) ..\Config\Debug & false	*copy $(TargetPath) ..\Config\Release & false
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\SerialMo"
# PROP BASE Intermediate_Dir ".\SerialMo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# Begin Special Build Tool
TargetPath=.\Debug\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
PostBuild_Cmds=net stop seriallogger & false	*copy $(TargetPath) ..\Config\Debug & false	*copy $(TargetPath) ..\Config\Release & false
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SerialMon___Win32_MCD_Debug"
# PROP BASE Intermediate_Dir "SerialMon___Win32_MCD_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SerialMon___Win32_MCD_Debug"
# PROP Intermediate_Dir "SerialMon___Win32_MCD_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "SERLOG_MCD" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# Begin Special Build Tool
TargetPath=.\SerialMon___Win32_MCD_Debug\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
PostBuild_Cmds=net stop seriallogger & false	*copy $(TargetPath) ..\Config\Debug & false	*copy $(TargetPath) ..\Config\Release & false
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SerialMon___Win32_MCD_Release"
# PROP BASE Intermediate_Dir "SerialMon___Win32_MCD_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SerialMon___Win32_MCD_Release"
# PROP Intermediate_Dir "SerialMon___Win32_MCD_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "SERLOG_MCD" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ENDIF 

# Begin Target

# Name "SerialMon - Win32 Release"
# Name "SerialMon - Win32 Debug"
# Name "SerialMon - Win32 MCD Debug"
# Name "SerialMon - Win32 MCD Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\CSmtp\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\BuildTimestamp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\InternalRevisionHistory.html
# End Source File
# Begin Source File

SOURCE=..\..\..\RegCode\RegCodeCommon.cpp

!IF  "$(CFG)" == "SerialMon - Win32 Release"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SerialLogger.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SerialLoggerCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialLoggerMsg.mc

!IF  "$(CFG)" == "SerialMon - Win32 Release"

# Begin Custom Build
InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

"$(InputName).rc $(InputName).h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"

# Begin Custom Build
InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

"$(InputName).rc $(InputName).h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

# Begin Custom Build
InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

"$(InputName).rc $(InputName).h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

# Begin Custom Build
InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

"$(InputName).rc $(InputName).h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SerialLoggerRegistry.txt
# End Source File
# Begin Source File

SOURCE=.\Service.cpp
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Smtp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ToDo.txt
# End Source File
# Begin Source File

SOURCE=.\Utility.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\CSmtp\Base64.h
# End Source File
# Begin Source File

SOURCE=..\Common\BuildTimestamp.h
# End Source File
# Begin Source File

SOURCE=..\Common\CommonResource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\RegCode\RegCodeCommon.h
# End Source File
# Begin Source File

SOURCE=.\SerialLogger.h
# End Source File
# Begin Source File

SOURCE=..\Common\SerialLoggerCommon.h
# End Source File
# Begin Source File

SOURCE=.\SerialLoggerMsg.h
# End Source File
# Begin Source File

SOURCE=..\CSmtp\Smtp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\SerialLogger.rc
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\MCD\License.txt
# End Source File
# End Target
# End Project
