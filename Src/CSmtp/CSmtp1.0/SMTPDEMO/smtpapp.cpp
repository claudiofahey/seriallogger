// smtpapp.cpp : Implementation of WinMain
//
// Written by Robert Simpson (robert@blackcastlesoft.com)
// Created 11/1/2000
// Version 1.0 -- Last Modified 11/02/2000
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "smtp.h"

CExeModule _Module;

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
  lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
  
  HRESULT hRes = CoInitialize(NULL);
  _ASSERTE(SUCCEEDED(hRes));

  _Module.Init(NULL,hInstance);
  _Module.dwThreadID = GetCurrentThreadId();  

  WSADATA wsa;
  WSAStartup(MAKEWORD(2,0),&wsa);

  CSmtp mail;
  CSmtpMessage msg;
  CSmtpAddress cc;
  
//  mail.m_bstrUser = "smtpuser";
//  mail.m_bstrPass = "smtppassword";

  msg.Sender.Address = "robert@blackcastlesoft.com";
  msg.Sender.Name = "Robert Simpson";

  msg.Recipient.Name = "Some User";
  msg.Recipient.Address = "someuser@somewhere.com";

/*
  cc.Name = "Some Other User";
  cc.Address = "someoneelse@somewhere.com";
  msg.CC.Add(cc);

  cc.Name = "Yet Another CC";
  cc.Address = "YACC@somewhere.com";
  msg.BCC.Add(cc);
*/

  msg.Subject = "Testing";

  msg.Message = "This is a test of the emergency broadcast system.  The broadcasters in your area, in voluntary cooperation with the FCC and other local authorities have developed this test to keep you informed of an emergency.\r\nHad this been an actual emergency, this message would have been followed by official news or instructions.  This concludes this test of the emergency broadcast system. \r\n ";

//  msg.Attachments.Add(CComBSTR("c:\\autoexec.bat"));

  if (mail.Connect("mail.blackcastlesoft.com"))
  {
    mail.SendMessage(msg);
    mail.Close();
  }


//  MSG msg;
//  while (GetMessage(&msg, 0, 0, 0))
//    DispatchMessage(&msg);
  
  _Module.Term();
  CoUninitialize();

  WSACleanup();

  return 0;
}
