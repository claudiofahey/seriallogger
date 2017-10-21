
MAKE=make

all:
	touch Src\Common\BuildTimestamp.cpp

	cd Src\Service
	$(MAKE) /f SerialLogger.mak CFG="SerialMon - Win32 Release"	
	cd ..\..

	cd Src\Config
	$(MAKE) /f SerialLoggerConfig.mak CFG="SMConfig - Win32 Release"
	cd ..\..

	cd HtmlHelp
	$(MAKE)
	cd ..

	cd InnoSetup
	iscc ScriptMaker.iss
	cd ..
