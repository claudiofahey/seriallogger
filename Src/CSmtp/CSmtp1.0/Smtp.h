// Smtp.h: interface for the CSmtp class.
//
// Written by Robert Simpson (robert@blackcastlesoft.com)
// Created 11/1/2000
// Version 1.0 -- Last Modified 11/02/2000
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMTP_H__F5ACA8FA_AF73_11D4_907D_0080C6F7C752__INCLUDED_)
#define AFX_SMTP_H__F5ACA8FA_AF73_11D4_907D_0080C6F7C752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base64.h"
#include <winsock.h>

// When the SMTP server responds to a command, this is the
// maximum size of a response I can expect back.
#define CMD_RESPONSE_SIZE 1024

class CSmtp;
class CSmtpAddress;
class CSmtpMessage;

// The textual part of a message can be encoded using one of these 3 methods
typedef enum TransferEncoding
{
  encode7Bit,
  encodeQuotedPrintable,
  encodeBase64
};

// This class represents a user's text name and corresponding email address
class CSmtpAddress
{
public:
  CComBSTR Name;
  CComBSTR Address;
};

// This class represents a single message that can be sent via CSmtp
class CSmtpMessage
{
public:
  CSmtpMessage();

public:
  CSmtpAddress                  Sender;        // Who the message is from
  //CSmtpAddress                Recipient;     // The intended recipient
  CSimpleArray<CSmtpAddress>    Recipient;     // The intended recipient - !CRF!
  CComBSTR                      Subject;       // The message subject
  CComBSTR                      Message;       // The message body
  CSimpleArray<CSmtpAddress>    CC;            // Carbon Copy recipients
  CSimpleArray<CSmtpAddress>    BCC;           // Blind Carbon Copy recipients
  CSimpleArray<CComBSTR>        Attachments;   // An array of filenames (full path) to include as attachments
  CSimpleMap<CComBSTR,CComBSTR> Headers;       // Optional headers to include in the message
  SYSTEMTIME                    Timestamp;     // Timestamp of the message
  TransferEncoding              MessageEncode; // How to encode the text part of the message

private:
  int                           GMTOffset;     // GMT timezone offset value

public:
  BSTR Parse();

private: // Private functions to finalize the message headers
  void CommitHeaders();
	BSTR BreakMessage(BSTR bstrSrc, int nLength = 76);
	BSTR EncodeQuotedPrintable(BSTR bstrSrc);
};

// The main class for connecting to a SMTP server and sending mail.
class CSmtp  
{
public:
	CSmtp();
	virtual ~CSmtp();

public: // Feel free to modify these to change the system's behavior
  BOOL     m_bExtensions;     // Use ESMTP extensions (TRUE)
  DWORD    m_dwCmdTimeout;    // Timeout for issuing each command (30)
  WORD     m_wSmtpPort;       // Port to communicate via SMTP (25)
  CComBSTR m_bstrUser;        // Username for authentication
  CComBSTR m_bstrPass;        // Password for authentication

private:
  SOCKET   m_hSocket;         // Socket being used to communicate via SMTP
  CComBSTR m_bstrResult;      // String result from a SendCmd()
  BOOL     m_bConnected;      // Connected to SMTP server
  BOOL     m_bUsingExtensions;// Whether this SMTP server uses ESMTP extensions

public: // These represent the primary public functionality of this class
	BOOL Connect(LPTSTR pszServer);
	int  SendMessage(CSmtpMessage& msg);
	void Close();

public: // These represent the overridable methods for receiving events from this class
	virtual int  SmtpWarning(int nWarning, LPTSTR pszWarning);
  virtual int  SmtpError(int nCode, LPTSTR pszErr);
	virtual void SmtpCommandResponse(LPTSTR pszCmd, int nResponse, LPTSTR pszResponse);

private: // These functions are used privately to conduct a SMTP session
	int  SendCmd(LPTSTR pszCmd);
	int  SendAuthentication();
	int  SendHello();
	int  SendQuitCmd();
	int  SendFrom(LPTSTR pszFrom);
	int  SendTo(LPTSTR pszTo);
	int  SendData(CSmtpMessage &msg);
  int  RaiseWarning(int nWarning);
	int  RaiseError(int nError);
};

#endif // !defined(AFX_SMTP_H__F5ACA8FA_AF73_11D4_907D_0080C6F7C752__INCLUDED_)
