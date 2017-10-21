# Microsoft Developer Studio Generated NMAKE File, Based on SerialLoggerConfig.dsp
!IF "$(CFG)" == ""
CFG=SMConfig - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SMConfig - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SMConfig - Win32 Release" && "$(CFG)" != "SMConfig - Win32 Debug" && "$(CFG)" != "SMConfig - Win32 Release with Symbols" && "$(CFG)" != "SMConfig - Win32 MCD Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SMConfig - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SerialLoggerConfig.exe" "$(OUTDIR)\SerialLoggerConfig.pch" "$(OUTDIR)\SerialLoggerConfig.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\CATSETS.OBJ"
	-@erase "$(INTDIR)\CATSETS.SBR"
	-@erase "$(INTDIR)\EmailPage.obj"
	-@erase "$(INTDIR)\EmailPage.sbr"
	-@erase "$(INTDIR)\GeneralPage.obj"
	-@erase "$(INTDIR)\GeneralPage.sbr"
	-@erase "$(INTDIR)\InstanceListDoc.obj"
	-@erase "$(INTDIR)\InstanceListDoc.sbr"
	-@erase "$(INTDIR)\InstanceListView.obj"
	-@erase "$(INTDIR)\InstanceListView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\OdbcPage.obj"
	-@erase "$(INTDIR)\OdbcPage.sbr"
	-@erase "$(INTDIR)\RegCodeCommon.obj"
	-@erase "$(INTDIR)\RegCodeCommon.sbr"
	-@erase "$(INTDIR)\RegDlg.obj"
	-@erase "$(INTDIR)\RegDlg.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\SerialLoggerConfig.pch"
	-@erase "$(INTDIR)\SMConfig.obj"
	-@erase "$(INTDIR)\SMConfig.res"
	-@erase "$(INTDIR)\SMConfig.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TableDialog.obj"
	-@erase "$(INTDIR)\TableDialog.sbr"
	-@erase "$(INTDIR)\TimeoutPage.obj"
	-@erase "$(INTDIR)\TimeoutPage.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SerialLoggerConfig.bsc"
	-@erase "$(OUTDIR)\SerialLoggerConfig.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SMConfig.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLoggerConfig.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\CATSETS.SBR" \
	"$(INTDIR)\EmailPage.sbr" \
	"$(INTDIR)\GeneralPage.sbr" \
	"$(INTDIR)\InstanceListDoc.sbr" \
	"$(INTDIR)\InstanceListView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\OdbcPage.sbr" \
	"$(INTDIR)\RegCodeCommon.sbr" \
	"$(INTDIR)\RegDlg.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\SMConfig.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TableDialog.sbr" \
	"$(INTDIR)\TimeoutPage.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\SerialLoggerConfig.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SerialLoggerConfig.pdb" /machine:I386 /out:"$(OUTDIR)\SerialLoggerConfig.exe" /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\CATSETS.OBJ" \
	"$(INTDIR)\EmailPage.obj" \
	"$(INTDIR)\GeneralPage.obj" \
	"$(INTDIR)\InstanceListDoc.obj" \
	"$(INTDIR)\InstanceListView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OdbcPage.obj" \
	"$(INTDIR)\RegCodeCommon.obj" \
	"$(INTDIR)\RegDlg.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\SMConfig.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableDialog.obj" \
	"$(INTDIR)\TimeoutPage.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\SMConfig.res"

"$(OUTDIR)\SerialLoggerConfig.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SerialLoggerConfig.exe" "$(OUTDIR)\SerialLoggerConfig.pch" "$(OUTDIR)\SerialLoggerConfig.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\CATSETS.OBJ"
	-@erase "$(INTDIR)\CATSETS.SBR"
	-@erase "$(INTDIR)\EmailPage.obj"
	-@erase "$(INTDIR)\EmailPage.sbr"
	-@erase "$(INTDIR)\GeneralPage.obj"
	-@erase "$(INTDIR)\GeneralPage.sbr"
	-@erase "$(INTDIR)\InstanceListDoc.obj"
	-@erase "$(INTDIR)\InstanceListDoc.sbr"
	-@erase "$(INTDIR)\InstanceListView.obj"
	-@erase "$(INTDIR)\InstanceListView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\OdbcPage.obj"
	-@erase "$(INTDIR)\OdbcPage.sbr"
	-@erase "$(INTDIR)\RegCodeCommon.obj"
	-@erase "$(INTDIR)\RegCodeCommon.sbr"
	-@erase "$(INTDIR)\RegDlg.obj"
	-@erase "$(INTDIR)\RegDlg.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\SerialLoggerConfig.pch"
	-@erase "$(INTDIR)\SMConfig.obj"
	-@erase "$(INTDIR)\SMConfig.res"
	-@erase "$(INTDIR)\SMConfig.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TableDialog.obj"
	-@erase "$(INTDIR)\TableDialog.sbr"
	-@erase "$(INTDIR)\TimeoutPage.obj"
	-@erase "$(INTDIR)\TimeoutPage.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SerialLoggerConfig.bsc"
	-@erase "$(OUTDIR)\SerialLoggerConfig.exe"
	-@erase "$(OUTDIR)\SerialLoggerConfig.ilk"
	-@erase "$(OUTDIR)\SerialLoggerConfig.map"
	-@erase "$(OUTDIR)\SerialLoggerConfig.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SMConfig.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLoggerConfig.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\CATSETS.SBR" \
	"$(INTDIR)\EmailPage.sbr" \
	"$(INTDIR)\GeneralPage.sbr" \
	"$(INTDIR)\InstanceListDoc.sbr" \
	"$(INTDIR)\InstanceListView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\OdbcPage.sbr" \
	"$(INTDIR)\RegCodeCommon.sbr" \
	"$(INTDIR)\RegDlg.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\SMConfig.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TableDialog.sbr" \
	"$(INTDIR)\TimeoutPage.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\SerialLoggerConfig.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SerialLoggerConfig.pdb" /map:"$(INTDIR)\SerialLoggerConfig.map" /debug /machine:I386 /out:"$(OUTDIR)\SerialLoggerConfig.exe" /pdbtype:sept /libpath:"D:\Program Files\HTML Help Workshop\lib" 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\CATSETS.OBJ" \
	"$(INTDIR)\EmailPage.obj" \
	"$(INTDIR)\GeneralPage.obj" \
	"$(INTDIR)\InstanceListDoc.obj" \
	"$(INTDIR)\InstanceListView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OdbcPage.obj" \
	"$(INTDIR)\RegCodeCommon.obj" \
	"$(INTDIR)\RegDlg.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\SMConfig.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableDialog.obj" \
	"$(INTDIR)\TimeoutPage.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\SMConfig.res"

"$(OUTDIR)\SerialLoggerConfig.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SMConfig - Win32 Release with Symbols"

OUTDIR=.\SymbolRelease
INTDIR=.\SymbolRelease
# Begin Custom Macros
OutDir=.\SymbolRelease
# End Custom Macros

ALL : "$(OUTDIR)\SerialLoggerConfig.exe" "$(OUTDIR)\SerialLoggerConfig.pch" "$(OUTDIR)\SerialLoggerConfig.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\CATSETS.OBJ"
	-@erase "$(INTDIR)\CATSETS.SBR"
	-@erase "$(INTDIR)\EmailPage.obj"
	-@erase "$(INTDIR)\EmailPage.sbr"
	-@erase "$(INTDIR)\GeneralPage.obj"
	-@erase "$(INTDIR)\GeneralPage.sbr"
	-@erase "$(INTDIR)\InstanceListDoc.obj"
	-@erase "$(INTDIR)\InstanceListDoc.sbr"
	-@erase "$(INTDIR)\InstanceListView.obj"
	-@erase "$(INTDIR)\InstanceListView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\OdbcPage.obj"
	-@erase "$(INTDIR)\OdbcPage.sbr"
	-@erase "$(INTDIR)\RegCodeCommon.obj"
	-@erase "$(INTDIR)\RegCodeCommon.sbr"
	-@erase "$(INTDIR)\RegDlg.obj"
	-@erase "$(INTDIR)\RegDlg.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\SerialLoggerConfig.pch"
	-@erase "$(INTDIR)\SMConfig.obj"
	-@erase "$(INTDIR)\SMConfig.res"
	-@erase "$(INTDIR)\SMConfig.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TableDialog.obj"
	-@erase "$(INTDIR)\TableDialog.sbr"
	-@erase "$(INTDIR)\TimeoutPage.obj"
	-@erase "$(INTDIR)\TimeoutPage.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SerialLoggerConfig.bsc"
	-@erase "$(OUTDIR)\SerialLoggerConfig.exe"
	-@erase "$(OUTDIR)\SerialLoggerConfig.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SMConfig.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLoggerConfig.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\CATSETS.SBR" \
	"$(INTDIR)\EmailPage.sbr" \
	"$(INTDIR)\GeneralPage.sbr" \
	"$(INTDIR)\InstanceListDoc.sbr" \
	"$(INTDIR)\InstanceListView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\OdbcPage.sbr" \
	"$(INTDIR)\RegCodeCommon.sbr" \
	"$(INTDIR)\RegDlg.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\SMConfig.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TableDialog.sbr" \
	"$(INTDIR)\TimeoutPage.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\SerialLoggerConfig.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SerialLoggerConfig.pdb" /debug /debugtype:both /machine:I386 /out:"$(OUTDIR)\SerialLoggerConfig.exe" /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\CATSETS.OBJ" \
	"$(INTDIR)\EmailPage.obj" \
	"$(INTDIR)\GeneralPage.obj" \
	"$(INTDIR)\InstanceListDoc.obj" \
	"$(INTDIR)\InstanceListView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OdbcPage.obj" \
	"$(INTDIR)\RegCodeCommon.obj" \
	"$(INTDIR)\RegDlg.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\SMConfig.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableDialog.obj" \
	"$(INTDIR)\TimeoutPage.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\SMConfig.res"

"$(OUTDIR)\SerialLoggerConfig.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SMConfig - Win32 MCD Release"

OUTDIR=.\SMConfig___Win32_MCD_Release
INTDIR=.\SMConfig___Win32_MCD_Release
# Begin Custom Macros
OutDir=.\SMConfig___Win32_MCD_Release
# End Custom Macros

ALL : "$(OUTDIR)\SerialLoggerConfig.exe" "$(OUTDIR)\SerialLoggerConfig.pch" "$(OUTDIR)\SerialLoggerConfig.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BuildTimestamp.obj"
	-@erase "$(INTDIR)\BuildTimestamp.sbr"
	-@erase "$(INTDIR)\CATSETS.OBJ"
	-@erase "$(INTDIR)\CATSETS.SBR"
	-@erase "$(INTDIR)\EmailPage.obj"
	-@erase "$(INTDIR)\EmailPage.sbr"
	-@erase "$(INTDIR)\GeneralPage.obj"
	-@erase "$(INTDIR)\GeneralPage.sbr"
	-@erase "$(INTDIR)\InstanceListDoc.obj"
	-@erase "$(INTDIR)\InstanceListDoc.sbr"
	-@erase "$(INTDIR)\InstanceListView.obj"
	-@erase "$(INTDIR)\InstanceListView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\OdbcPage.obj"
	-@erase "$(INTDIR)\OdbcPage.sbr"
	-@erase "$(INTDIR)\RegDlg.obj"
	-@erase "$(INTDIR)\RegDlg.sbr"
	-@erase "$(INTDIR)\SerialLoggerCommon.obj"
	-@erase "$(INTDIR)\SerialLoggerCommon.sbr"
	-@erase "$(INTDIR)\SerialLoggerConfig.pch"
	-@erase "$(INTDIR)\SMConfig.obj"
	-@erase "$(INTDIR)\SMConfig.res"
	-@erase "$(INTDIR)\SMConfig.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TableDialog.obj"
	-@erase "$(INTDIR)\TableDialog.sbr"
	-@erase "$(INTDIR)\TimeoutPage.obj"
	-@erase "$(INTDIR)\TimeoutPage.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SerialLoggerConfig.bsc"
	-@erase "$(OUTDIR)\SerialLoggerConfig.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "SERLOG_MCD" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SMConfig.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SerialLoggerConfig.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BuildTimestamp.sbr" \
	"$(INTDIR)\CATSETS.SBR" \
	"$(INTDIR)\EmailPage.sbr" \
	"$(INTDIR)\GeneralPage.sbr" \
	"$(INTDIR)\InstanceListDoc.sbr" \
	"$(INTDIR)\InstanceListView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\OdbcPage.sbr" \
	"$(INTDIR)\RegDlg.sbr" \
	"$(INTDIR)\SerialLoggerCommon.sbr" \
	"$(INTDIR)\SMConfig.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TableDialog.sbr" \
	"$(INTDIR)\TimeoutPage.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\SerialLoggerConfig.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SerialLoggerConfig.pdb" /machine:I386 /out:"$(OUTDIR)\SerialLoggerConfig.exe" /libpath:"D:\Program Files\HTML Help Workshop\lib" /opt:ref,icf 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BuildTimestamp.obj" \
	"$(INTDIR)\CATSETS.OBJ" \
	"$(INTDIR)\EmailPage.obj" \
	"$(INTDIR)\GeneralPage.obj" \
	"$(INTDIR)\InstanceListDoc.obj" \
	"$(INTDIR)\InstanceListView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OdbcPage.obj" \
	"$(INTDIR)\RegDlg.obj" \
	"$(INTDIR)\SerialLoggerCommon.obj" \
	"$(INTDIR)\SMConfig.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableDialog.obj" \
	"$(INTDIR)\TimeoutPage.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\SMConfig.res"

"$(OUTDIR)\SerialLoggerConfig.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SerialLoggerConfig.dep")
!INCLUDE "SerialLoggerConfig.dep"
!ELSE 
!MESSAGE Warning: cannot find "SerialLoggerConfig.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SMConfig - Win32 Release" || "$(CFG)" == "SMConfig - Win32 Debug" || "$(CFG)" == "SMConfig - Win32 Release with Symbols" || "$(CFG)" == "SMConfig - Win32 MCD Release"
SOURCE=.\AboutDlg.cpp

"$(INTDIR)\AboutDlg.obj"	"$(INTDIR)\AboutDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\CSmtp\Base64.cpp

"$(INTDIR)\Base64.obj"	"$(INTDIR)\Base64.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\BuildTimestamp.cpp

"$(INTDIR)\BuildTimestamp.obj"	"$(INTDIR)\BuildTimestamp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CATSETS.CPP

"$(INTDIR)\CATSETS.OBJ"	"$(INTDIR)\CATSETS.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EmailPage.cpp

"$(INTDIR)\EmailPage.obj"	"$(INTDIR)\EmailPage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GeneralPage.cpp

"$(INTDIR)\GeneralPage.obj"	"$(INTDIR)\GeneralPage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\InstanceListDoc.cpp

"$(INTDIR)\InstanceListDoc.obj"	"$(INTDIR)\InstanceListDoc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\InstanceListView.cpp

"$(INTDIR)\InstanceListView.obj"	"$(INTDIR)\InstanceListView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OdbcPage.cpp

"$(INTDIR)\OdbcPage.obj"	"$(INTDIR)\OdbcPage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\..\RegCode\RegCodeCommon.cpp

!IF  "$(CFG)" == "SMConfig - Win32 Release"


"$(INTDIR)\RegCodeCommon.obj"	"$(INTDIR)\RegCodeCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SMConfig - Win32 Debug"


"$(INTDIR)\RegCodeCommon.obj"	"$(INTDIR)\RegCodeCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SMConfig - Win32 Release with Symbols"


"$(INTDIR)\RegCodeCommon.obj"	"$(INTDIR)\RegCodeCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SMConfig - Win32 MCD Release"

!ENDIF 

SOURCE=.\RegDlg.cpp

"$(INTDIR)\RegDlg.obj"	"$(INTDIR)\RegDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\SerialLoggerCommon.cpp

"$(INTDIR)\SerialLoggerCommon.obj"	"$(INTDIR)\SerialLoggerCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\SMConfig.cpp

"$(INTDIR)\SMConfig.obj"	"$(INTDIR)\SMConfig.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\CSmtp\Smtp.cpp

"$(INTDIR)\Smtp.obj"	"$(INTDIR)\Smtp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SMConfig - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SerialLoggerConfig.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SMConfig - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SerialLoggerConfig.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SMConfig - Win32 Release with Symbols"

CPP_SWITCHES=/nologo /MT /W3 /GX /Zi /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SerialLoggerConfig.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SMConfig - Win32 MCD Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "D:\Program Files\HTML Help Workshop\include" /D "SERLOG_MCD" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CONFIGAPP" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SerialLoggerConfig.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SerialLoggerConfig.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TableDialog.cpp

"$(INTDIR)\TableDialog.obj"	"$(INTDIR)\TableDialog.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TimeoutPage.cpp

"$(INTDIR)\TimeoutPage.obj"	"$(INTDIR)\TimeoutPage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Utilities.cpp

"$(INTDIR)\Utilities.obj"	"$(INTDIR)\Utilities.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SMConfig.rc

"$(INTDIR)\SMConfig.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

