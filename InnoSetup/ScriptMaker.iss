; Inno Setup Script
; Created with ScriptMaker Version 2.0.10
; 9 July 2003 at 22:55

[Setup]
    MinVersion=0,4.0
    AdminPrivilegesRequired=1
    AppName=Serial Logger Service 1.21e
    AppId=Serial Logger Service
    CreateUninstallRegKey=1
    UsePreviousAppDir=1
    UsePreviousGroup=1
    AppPublisher=Mu-Consulting
    AppPublisherURL=http://www.muconsulting.com
    AppVersion=1.21e
    AppVerName=Serial Logger Service 1.21e
    AppCopyright=Copyright © 2004 by Mu-Consulting
    WizardStyle=modern
    UninstallStyle=modern
    WindowShowCaption=1
    WindowStartMaximized=1
    WindowVisible=0
    WindowResizable=1
    UninstallDisplayIcon={app}\SerialLoggerConfig.exe
    UninstallLogMode=Append
    DirExistsWarning=auto
    UninstallFilesDir={app}
    DisableDirPage=0
    DisableStartupPrompt=1
    CreateAppDir=1
    DisableProgramGroupPage=0
    ;AlwaysCreateUninstallIcon=1
    ;UninstallIconName=Uninstall Serial Logger Service
    Uninstallable=1
    DefaultDirName={pf}\Serial Logger Service
    DefaultGroupName=Serial Logger Service
    LicenseFile=D:\MUConsulting\SerialLogger\LicenseAgreement.doc
    OutputBaseFilename=serialloggersetup
    ;MessagesFile=compiler:default.isl
    ShowComponentSizes=1
    SourceDir=d:\MUConsulting\SerialLogger
    OutputDir=d:\MUConsulting\SerialLogger\InnoSetup\Output


[Files]
    Source: D:\MUConsulting\SerialLogger\Src\Service\Release\SerialLogger.exe; DestDir: {app}\; DestName: SerialLogger.exe
    Source: D:\MUConsulting\SerialLogger\Src\Config\Release\SerialLoggerConfig.exe; DestDir: {app}\; DestName: SerialLoggerConfig.exe
    Source: D:\MUConsulting\SerialLogger\HtmlHelp\SerialLogger.chm; DestDir: {app}\; DestName: SerialLogger.chm

[Icons]
    Name: {group}\Serial Logger Service Help; Filename: {app}\SerialLogger.chm; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
    Name: {group}\Serial Logger Service Configuration; Filename: {app}\SerialLoggerConfig.exe; WorkingDir: {app}; IconFilename: {app}\SerialLoggerConfig.exe; IconIndex: 0

[INI]

[Registry]

[UninstallDelete]

[InstallDelete]

[Run]
    Filename: {app}\SerialLoggerConfig.exe; Flags: waituntilidle postinstall; WorkingDir: {app}; Description: Configure Serial Logger Service

[UninstallRun]
    Filename: {app}\SerialLoggerConfig.exe; Parameters: /removeall; WorkingDir: {app}


; ==============================================
; The lines below are used by ScriptMaker
; They are not required by Inno Setup
; DO NOT DELETE THEM or you may be unable to reload the script

;[ScriptSetup]
;VerNum=1.21e
;InnoVer=2.0
;AddVerTo=AppName AppVerName
;SetupFilename=serialloggersetup.exe
;OutputFolder=d:\MUConsulting\SerialLogger\InnoSetup\Output
;SetupIconFileName=D:\MUConsulting\SerialLogger\SerialLoggerConfig\Release\SerialLoggerConfig.exe
;SetupIconIndex=-1
;CopyrightText=Copyright © 2004 by Mu-Consulting

