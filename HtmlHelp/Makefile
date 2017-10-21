
MAKE=make

all:
	makehm ID_,IDH_,0x10000  \
		   IDM_,IDH_,0x10000 \
		   IDD_,IDH_,0x20000 \
		   IDP_,IDH_,0x30000 \
		   IDR_,IDH_,0x20000 \
		   IDW_,IDH_,0x50000 \
		   ..\src\config\resource.h SerialLogger.hm1
	awk "{print \"#define \" $$0}" SerialLogger.hm1 > SerialLogger.hm
	-hhc SerialLogger.hhp
	-*copy SerialLogger.chm ..\src\config\debug
	-*copy SerialLogger.chm ..\src\config\release