# Microsoft Developer Studio Generated NMAKE File, Based on SerialLogger.dsp
!IF "$(CFG)" == ""
CFG=SerialMon - Win32 MCD Debug
!MESSAGE No configuration specified. Defaulting to SerialMon - Win32 MCD Debug.
!ENDIF 

!IF "$(CFG)" != "SerialMon - Win32 Release" && "$(CFG)" != "SerialMon - Win32 Debug" && "$(CFG)" != "SerialMon - Win32 MCD Debug" && "$(CFG)" != "SerialMon - Win32 MCD Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SerialMon - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\RegCodeCommon.obj"
	-@erase "$(INTDIR)\RegCodeCommon.sbr"
	-@erase "$(INTDIR)\SerialLogger.obj"
	-@erase "$(INTDIR)\SerialLogger.res"
	-@erase "$(INTDIR)\SerialLogger.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\Service.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SerialLogger.bsc"
	-@erase "$(OUTDIR)\SerialLogger.exe"
	-@erase "SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLogger.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SerialLogger.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLogger.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\RegCodeCommon.sbr" \
	"$(INTDIR)\SerialLogger.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\Service.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\Utility.sbr"

"$(OUTDIR)\SerialLogger.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\SerialLogger.pdb" /machine:I386 /out:"$(OUTDIR)\SerialLogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\RegCodeCommon.obj" \
	"$(INTDIR)\SerialLogger.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\SerialLogger.res"

"$(OUTDIR)\SerialLogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Release\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"
   net stop seriallogger & false
	*copy .\Release\SerialLogger.exe ..\Config\Debug & false
	*copy .\Release\SerialLogger.exe ..\Config\Release & false
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\RegCodeCommon.obj"
	-@erase "$(INTDIR)\RegCodeCommon.sbr"
	-@erase "$(INTDIR)\SerialLogger.obj"
	-@erase "$(INTDIR)\SerialLogger.res"
	-@erase "$(INTDIR)\SerialLogger.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\Service.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SerialLogger.bsc"
	-@erase "$(OUTDIR)\SerialLogger.exe"
	-@erase "$(OUTDIR)\SerialLogger.ilk"
	-@erase "$(OUTDIR)\SerialLogger.pdb"
	-@erase "SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLogger.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SerialLogger.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLogger.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\RegCodeCommon.sbr" \
	"$(INTDIR)\SerialLogger.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\Service.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\Utility.sbr"

"$(OUTDIR)\SerialLogger.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\SerialLogger.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SerialLogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\RegCodeCommon.obj" \
	"$(INTDIR)\SerialLogger.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\SerialLogger.res"

"$(OUTDIR)\SerialLogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\Debug\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"
   net stop seriallogger & false
	*copy .\Debug\SerialLogger.exe ..\Config\Debug & false
	*copy .\Debug\SerialLogger.exe ..\Config\Release & false
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

OUTDIR=.\SerialMon___Win32_MCD_Debug
INTDIR=.\SerialMon___Win32_MCD_Debug
# Begin Custom Macros
OutDir=.\SerialMon___Win32_MCD_Debug
# End Custom Macros

ALL : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\SerialLogger.obj"
	-@erase "$(INTDIR)\SerialLogger.res"
	-@erase "$(INTDIR)\SerialLogger.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\Service.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SerialLogger.bsc"
	-@erase "$(OUTDIR)\SerialLogger.exe"
	-@erase "$(OUTDIR)\SerialLogger.ilk"
	-@erase "$(OUTDIR)\SerialLogger.pdb"
	-@erase "SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "SERLOG_MCD" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLogger.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SerialLogger.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLogger.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\SerialLogger.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\Service.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\Utility.sbr"

"$(OUTDIR)\SerialLogger.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\SerialLogger.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SerialLogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\SerialLogger.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\SerialLogger.res"

"$(OUTDIR)\SerialLogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=.\SerialMon___Win32_MCD_Debug\SerialLogger.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Running Post-build commands...
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\SerialMon___Win32_MCD_Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"
   net stop seriallogger & false
	*copy .\SerialMon___Win32_MCD_Debug\SerialLogger.exe ..\Config\Debug & false
	*copy .\SerialMon___Win32_MCD_Debug\SerialLogger.exe ..\Config\Release & false
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

OUTDIR=.\SerialMon___Win32_MCD_Release
INTDIR=.\SerialMon___Win32_MCD_Release
# Begin Custom Macros
OutDir=.\SerialMon___Win32_MCD_Release
# End Custom Macros

ALL : ".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" "$(OUTDIR)\SerialLogger.exe" "$(OUTDIR)\SerialLogger.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\SerialLogger.obj"
	-@erase "$(INTDIR)\SerialLogger.res"
	-@erase "$(INTDIR)\SerialLogger.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\Service.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SerialLogger.bsc"
	-@erase "$(OUTDIR)\SerialLogger.exe"
	-@erase "SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "SERLOG_MCD" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D _WIN32_WINNT=0x400 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLogger.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SerialLogger.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLogger.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\SerialLogger.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\Service.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\Utility.sbr"

"$(OUTDIR)\SerialLogger.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\SerialLogger.pdb" /machine:I386 /out:"$(OUTDIR)\SerialLogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\SerialLogger.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\SerialLogger.res"

"$(OUTDIR)\SerialLogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SerialLogger.dep")
!INCLUDE "SerialLogger.dep"
!ELSE 
!MESSAGE Warning: cannot find "SerialLogger.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SerialMon - Win32 Release" || "$(CFG)" == "SerialMon - Win32 Debug" || "$(CFG)" == "SerialMon - Win32 MCD Debug" || "$(CFG)" == "SerialMon - Win32 MCD Release"
SOURCE=..\CSmtp\Base64.cpp

"$(INTDIR)\Base64.obj"	"$(INTDIR)\Base64.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\BuildTimestamp.cpp

"$(INTDIR)\BuildTimestamp.obj"	"$(INTDIR)\BuildTimestamp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\RegCode\RegCodeCommon.cpp

!IF  "$(CFG)" == "SerialMon - Win32 Release"


"$(INTDIR)\RegCodeCommon.obj"	"$(INTDIR)\RegCodeCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"


"$(INTDIR)\RegCodeCommon.obj"	"$(INTDIR)\RegCodeCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

!ENDIF 

SOURCE=.\SerialLogger.cpp

"$(INTDIR)\SerialLogger.obj"	"$(INTDIR)\SerialLogger.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\SerialLoggerCommon.cpp

"$(INTDIR)\SerialLoggerCommon.obj"	"$(INTDIR)\SerialLoggerCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\SerialLoggerMsg.mc

!IF  "$(CFG)" == "SerialMon - Win32 Release"

InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	mc $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "SerialMon - Win32 Debug"

InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	mc $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Debug"

InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	mc $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "SerialMon - Win32 MCD Release"

InputPath=.\SerialLoggerMsg.mc
InputName=SerialLoggerMsg

".\SerialLoggerMsg.rc SerialLoggerMsg.h msg0001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	mc $(InputPath)
<< 
	

!ENDIF 

SOURCE=.\Service.cpp

"$(INTDIR)\Service.obj"	"$(INTDIR)\Service.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\CSmtp\Smtp.cpp

"$(INTDIR)\Smtp.obj"	"$(INTDIR)\Smtp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility.cpp

"$(INTDIR)\Utility.obj"	"$(INTDIR)\Utility.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SerialLogger.rc

"$(INTDIR)\SerialLogger.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

