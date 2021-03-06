; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COdbcPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "smconfig.h"
LastPage=0

ClassCount=12
Class1=CAboutDlg
Class2=CEmailPage
Class3=CGeneralPage
Class4=CMainWnd
Class5=CRegDlg
Class6=CSMConfigApp
Class7=CTimeoutPage

ResourceCount=9
Resource1=IDD_TABLELIST
Resource2=IDD_ODBCPAGE
Resource3=IDD_REGDLG
Resource4=IDM_POPUPS
Resource5=IDD_TIMEOUTPAGE
Resource6=IDD_GENERALPAGE
Class8=CMainFrame
Class9=CInstanceListView
Class10=CInstanceListDoc
Resource7=IDD_EMAILPAGE
Class11=COdbcPage
Resource8=IDD_ABOUTBOX
Class12=CTableDialog
Resource9=IDR_MAINFRAME

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
LastObject=CAboutDlg
Filter=D

[CLS:CEmailPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=EmailPage.h
ImplementationFile=EmailPage.cpp
Filter=D
LastObject=IDC_SENDNOW
VirtualFilter=idWC

[CLS:CGeneralPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GeneralPage.h
ImplementationFile=GeneralPage.cpp
Filter=D
LastObject=CGeneralPage
VirtualFilter=idWC

[CLS:CMainWnd]
Type=0
BaseClass=CDialog
HeaderFile=MainWnd.h
ImplementationFile=MainWnd.cpp
LastObject=CMainWnd
Filter=D
VirtualFilter=dWC

[CLS:CRegDlg]
Type=0
BaseClass=CDialog
HeaderFile=RegDlg.h
ImplementationFile=RegDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CRegDlg

[CLS:CSMConfigApp]
Type=0
BaseClass=CWinApp
HeaderFile=SMConfig.h
ImplementationFile=SMConfig.cpp
Filter=N
LastObject=CSMConfigApp

[CLS:CTimeoutPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TimeoutPage.h
ImplementationFile=TimeoutPage.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_ABOUTAPPNAME,static,1342308480
Control3=IDC_ABOUTCOPYRIGHT,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_ABOUTREGNAME,static,1342308352
Control7=IDC_ABOUTWEBSITE,static,1342308608
Control8=IDC_ABOUTDATE,static,1342308480

[DLG:IDD_EMAILPAGE]
Type=1
Class=CEmailPage
ControlCount=8
Control1=IDC_STATIC,static,1342308864
Control2=IDC_SMTPSERVER,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_SMTPFROMADDRESS,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_SMTPTOADDRESS,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_SENDNOW,button,1342242816

[DLG:IDD_GENERALPAGE]
Type=1
Class=CGeneralPage
ControlCount=27
Control1=IDC_STATIC,static,1342308864
Control2=IDC_PORTNAME,combobox,1344339970
Control3=IDC_STATIC,static,1342308876
Control4=IDC_BAUDRATE,combobox,1344339970
Control5=IDC_STATIC,static,1342308876
Control6=IDC_BYTESIZE,combobox,1344339971
Control7=IDC_STATIC,static,1342308876
Control8=IDC_PARITY,combobox,1344339971
Control9=IDC_STATIC,static,1342308876
Control10=IDC_STOPBITS,combobox,1344339971
Control11=IDC_STATIC,static,1342308876
Control12=IDC_TIMESTAMPFORMAT,combobox,1344339971
Control13=IDC_STATIC,button,1342177287
Control14=IDC_LOGSTARTSTOP,button,1342242819
Control15=IDC_LOGCONFIG,button,1342242819
Control16=IDC_LOGTIMEOUTS,button,1342242819
Control17=IDC_LOGEMAIL,button,1342242819
Control18=IDC_STARTAUTOMATICALLY,button,1342242819
Control19=IDC_STATIC,static,1342308864
Control20=IDC_LOGFILENAME,edit,1350631552
Control21=IDC_LOGFILENAMEBTN,button,1342242816
Control22=IDC_LOGOPEN,button,1342242816
Control23=IDC_STATIC,static,1342308876
Control24=IDC_LOGROTATION,combobox,1344339971
Control25=IDC_STATIC,static,1342308876
Control26=IDC_LOGROTATIONFILE,static,1342312960
Control27=IDC_STATIC,static,1342308352

[DLG:IDD_REGDLG]
Type=1
Class=CRegDlg
ControlCount=12
Control1=IDC_REGDLGNAME,edit,1350631552
Control2=IDC_REGDLGCODE,edit,1350631552
Control3=IDC_REGDLGVALIDATE,button,1342373888
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342308352
Control7=IDC_REGDLGCURREGNAME,static,1342308352
Control8=IDC_REGDLGWEBSITE,static,1342308608
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,static,1342308864

[DLG:IDD_TIMEOUTPAGE]
Type=1
Class=CTimeoutPage
ControlCount=31
Control1=IDC_TIMEOUTSECGRP,button,1342177287
Control2=IDC_NEVERTIMEOUT,button,1342373897
Control3=IDC_ALLOWTIMEOUT,button,1342177289
Control4=IDC_TIMEOUTSEC,edit,1350762624
Control5=IDC_TIMEOUTSECTEXT1,static,1342177280
Control6=IDC_WARNINGINTERVALGRP,button,1342177287
Control7=IDC_WARNINGINTERVALONCE,button,1342373897
Control8=IDC_WARNINGINTERVALREPEAT,button,1342177289
Control9=IDC_WARNINGINTERVALSEC,edit,1350762624
Control10=IDC_WARNINGINTERVALTEXT1,static,1342177280
Control11=IDC_TIMEOUTDAYSGRP,button,1342177287
Control12=IDC_TIMEOUTDAYSTEXT1,static,1342177280
Control13=IDC_SUNDAY,button,1342242819
Control14=IDC_MONDAY,button,1342242819
Control15=IDC_TUESDAY,button,1342242819
Control16=IDC_WEDNESDAY,button,1342242819
Control17=IDC_THURSDAY,button,1342242819
Control18=IDC_FRIDAY,button,1342242819
Control19=IDC_SATURDAY,button,1342242819
Control20=IDC_TIMEOUTTIMESGRP,button,1342177287
Control21=IDC_TIMEOUTTIMESTEXT1,static,1342177280
Control22=IDC_ENTIREDAY,button,1342242819
Control23=IDC_TIMEBEGIN,SysDateTimePick32,1342242857
Control24=IDC_TIMEOUTTIMESTEXT2,static,1342177280
Control25=IDC_TIMEEND,SysDateTimePick32,1342242857
Control26=IDC_TIMEOUTCMDGRP,button,1342177287
Control27=IDC_TIMEOUTCMDTEXT1,static,1342177792
Control28=IDC_TIMEOUTCMD,edit,1350631552
Control29=IDC_TIMEOUTCMDBROWSEBTN,button,1342242816
Control30=IDC_TIMEOUTCMDTEXT2,static,1342177792
Control31=IDC_NETSENDLIST,edit,1350631552

[MNU:IDM_POPUPS]
Type=1
Class=?
Command1=ID_SERVICE_START
Command2=ID_SERVICE_STOP
Command3=ID_SERVICE_RESTART
Command4=ID_SERVICE_CONTINUE
Command5=ID_SERVICE_PAUSE
Command6=ID_SERVICE_STARTAUTOMATICALLY
Command7=ID_EDIT_PROPERTIES
Command8=ID_EDIT_DUPLICATE
Command9=ID_EDIT_DELETE
Command10=ID_EDIT_NEW
CommandCount=10

[CLS:CInstanceListView]
Type=0
HeaderFile=InstanceListView.h
ImplementationFile=InstanceListView.cpp
BaseClass=CListView
Filter=C
LastObject=CInstanceListView
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
BaseClass=CFrameWnd
Filter=T
LastObject=CMainFrame
VirtualFilter=fWC

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_SAVE
Command2=ID_APP_EXIT
Command3=ID_EDIT_PROPERTIES
Command4=ID_EDIT_NEW
Command5=ID_EDIT_DUPLICATE
Command6=ID_EDIT_SELECTALL
Command7=ID_EDIT_DELETE
Command8=ID_SERVICE_START
Command9=ID_SERVICE_STOP
Command10=ID_SERVICE_RESTART
Command11=ID_SERVICE_CONTINUE
Command12=ID_SERVICE_PAUSE
Command13=ID_SERVICE_STARTAUTOMATICALLY
Command14=ID_EVENTVIEWER
Command15=ID_HELP
Command16=ID_REGISTER
Command17=ID_APP_ABOUT
CommandCount=17

[CLS:CInstanceListDoc]
Type=0
HeaderFile=InstanceListDoc.h
ImplementationFile=InstanceListDoc.cpp
BaseClass=CDocument
Filter=N
VirtualFilter=DC
LastObject=CInstanceListDoc

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_SELECTALL
Command2=ID_FILE_SAVE
Command3=ID_EDIT_DELETE
Command4=ID_EDIT_PROPERTIES
CommandCount=4

[DLG:IDD_ODBCPAGE]
Type=1
Class=COdbcPage
ControlCount=10
Control1=IDC_STATIC,static,1342308864
Control2=IDC_CONNECTSTRING,edit,1352728580
Control3=IDC_BROWSEDSN,button,1342242816
Control4=IDC_STATIC,static,1342308864
Control5=IDC_SQLSTATEMENT,edit,1352732676
Control6=IDC_BUILDSQLSTATEMENT,button,1342242816
Control7=IDC_TEST,button,1342242816
Control8=IDC_STATIC,static,1342308864
Control9=IDC_MAXLINELENGTH,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[CLS:COdbcPage]
Type=0
HeaderFile=OdbcPage.h
ImplementationFile=OdbcPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_TEST
VirtualFilter=idWC

[DLG:IDD_TABLELIST]
Type=1
Class=CTableDialog
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST,listbox,1352728835

[CLS:CTableDialog]
Type=0
HeaderFile=TableDialog.h
ImplementationFile=TableDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CTableDialog
VirtualFilter=dWC

