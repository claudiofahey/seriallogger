// Smtp.cpp: implementation of the CSmtp and CSmtpMessage classes
//
// Written by Robert Simpson (robert@blackcastlesoft.com)
// Created 11/1/2000
// Version 1.0 -- Last Modified 11/02/2000
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Smtp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtpMessage
//////////////////////////////////////////////////////////////////////
CSmtpMessage::CSmtpMessage()
{
  TIME_ZONE_INFORMATION tzi;
  DWORD dwRet;
  CComBSTR bstrHeader;
  CComBSTR bstrValue;
  
  // Get local time and timezone offset
  GetLocalTime(&Timestamp);
  GMTOffset = 0;
  dwRet = GetTimeZoneInformation(&tzi);
  GMTOffset = -(tzi.Bias * 100) / 60;

  // Default to quoted-printable message encoding
  // This encoding distinguishes between soft and hard line breaks
  MessageEncode = encodeQuotedPrintable;
  
  // Fill in standard headers
  // These are written out during intialization rather than
  // during a SendMessage() to allow the user to override
  // these values by re-assigning them before sending the message.
  bstrHeader = _T("MIME-Version");
  bstrValue  = _T("1.0");
  Headers.Add(bstrHeader,bstrValue);
  
  bstrHeader = _T("X-Priority");
  bstrValue  = _T("3 (Normal)");
  Headers.Add(bstrHeader,bstrValue);
  
  bstrHeader = _T("X-MSMail-Priority");
  bstrValue  = _T("Normal");
  Headers.Add(bstrHeader,bstrValue);
  
  bstrHeader = _T("X-Mailer");
  bstrValue  = _T("ATL CSmtp Class Mailer by Robert Simpson (robert@blackcastlesoft.com)");
  Headers.Add(bstrHeader,bstrValue);
  
  bstrHeader = _T("Importance");
  bstrValue  = _T("Normal");
  Headers.Add(bstrHeader,bstrValue);
}

// Write all the headers to the e-mail message.
// This is done just before sending it, when we're sure the user wants it to go out.
void CSmtpMessage::CommitHeaders()
{
  USES_CONVERSION;
  TCHAR szTime[64];
  TCHAR szDate[64];
  TCHAR szName[64];
  TCHAR szOut[1024];
  CComBSTR bstrHeader;
  CComBSTR bstrValue;
  DWORD dwSize = 64;
  CBase64 cvt;
  int n;

  // Get the time/date stamp and GMT offset for the Date header.
  GetDateFormat(LOCALE_SYSTEM_DEFAULT,0,&Timestamp,_T("ddd, d MMM yyyy"),szDate,64);
  GetTimeFormat(LOCALE_SYSTEM_DEFAULT,0,&Timestamp,_T("H:mm:ss"),szTime,64);
  
  wsprintf(szOut,_T("%s %s %4.4d"),szDate,szTime,GMTOffset);
  bstrHeader = _T("Date");
  bstrValue = szOut;
  Headers.Add(bstrHeader,bstrValue);

  // Write out the FROM header
  bstrValue.Empty();
  bstrHeader = _T("From");
  if (Sender.Name.Length())
  {
    wsprintf(szOut,_T("\"%s\" "),OLE2T(Sender.Name));
    bstrValue += szOut;
  }
  wsprintf(szOut,_T("<%s>"),OLE2T(Sender.Address));
  bstrValue += szOut;
  Headers.Add(bstrHeader,bstrValue);

  // Write out the TO header
  bstrValue.Empty();
  bstrHeader = _T("To");
  if (Recipient.Name.Length())
  {
    wsprintf(szOut,_T("\"%s\" "),OLE2T(Recipient.Name));
    bstrValue += szOut;
  }
  if (Recipient.Address.Length())
  {
    wsprintf(szOut,_T("<%s>"),OLE2T(Recipient.Address));
    bstrValue += szOut;
  }
  // Write out all the CC'd names
  for (n = 0;n < CC.GetSize();n++)
  {
    bstrValue += _T(",\r\n\t");
    if (CC[n].Name.Length())
    {
      wsprintf(szOut,_T("\"%s\" "),OLE2T(CC[n].Name));
      bstrValue += szOut;
    }
    wsprintf(szOut,_T("<%s>"),OLE2T(CC[n].Address));
    bstrValue += szOut;
  }
  Headers.Add(bstrHeader,bstrValue);

  // Write out the subject header
  bstrHeader = _T("Subject");
  bstrValue = OLE2T(Subject);
  Headers.Add(bstrHeader,bstrValue);

  // Create a unique message ID
  GetComputerName(szName,&dwSize);
  wsprintf(szOut,_T("%s%4.4X%2.2X%2.2X%8.8X"),szName,Timestamp.wYear,Timestamp.wMonth,Timestamp.wDay,GetTickCount());
  cvt.Encode(szOut);
  bstrHeader = _T("Message-ID");
  bstrValue = cvt.EncodedMessage();
  Headers.Add(bstrHeader,bstrValue);
}

// Parse a message into a single string
BSTR CSmtpMessage::Parse()
{
  USES_CONVERSION;
  CComBSTR bstrMessage;
  CComBSTR bstrHeader;
  CComBSTR bstrValue;
  CComBSTR bstrTemp;
  CComBSTR bstrBoundary;  
  TCHAR szOut[1024];

  int n;
  LPOLESTR pszFile;
  LPOLESTR pszMsgTok1;
  LPOLESTR pszMsgTok2;
  CBase64 cvt;

  bstrHeader = _T("Content-Type");
  Headers.Remove(bstrHeader);

  // Make sure the headers are properly accounted for
  CommitHeaders();

  // Get a count of the sections to see if this will be a multipart message
  n = (Message.Length() > 0);
  n += Attachments.GetSize();

  // If we have more than one section, then this is a multipart MIME message
  // In that case, create a boundary tag and set the content-type to multipart/mixed
  if (n > 1)
  {
    wsprintf(szOut,_T("CSmtpMsgPart123X456%8.8X"),GetTickCount());
    bstrBoundary = szOut;
    wsprintf(szOut,_T("multipart/mixed;\r\n\tboundary=\"%s\""),OLE2T(bstrBoundary));
    bstrValue = szOut;
    Headers.Add(bstrHeader,bstrValue);
  }
  else bstrBoundary.Empty(); // Clear out any boundary in case this message is being re-used.

  // Write out all the message headers
  for (n = 0;n < Headers.GetSize();n++)
  {
    bstrMessage += Headers.GetKeyAt(n);
    bstrMessage += _T(": ");
    bstrMessage += Headers.GetValueAt(n);
    bstrMessage += _T("\r\n");
  }

  // Process the message body, if there is any.
  if (Message.Length())
  {
    bstrValue.Empty();
    // If we're multipart, then write the boundary line
    if (bstrBoundary.Length())
    {
      bstrMessage += _T("\r\n");
      bstrMessage += _T("--");
      bstrMessage += bstrBoundary;
      bstrMessage += _T("\r\n");
    }
    // The first part of the message is the message text
    bstrMessage += _T("Content-Type: text/plain;\r\n\tcharset=\"iso-8859-1\"\r\n");
    
    // Parse the message text, replacing CRLF.CRLF sequences with CRLF..CRLF sequences
    pszMsgTok1 = Message.m_str;
    do
    {
      pszMsgTok2 = wcsstr(pszMsgTok1,OLESTR("\r\n.\r\n"));
      if (pszMsgTok2)
      {
        bstrValue.Append(pszMsgTok1,pszMsgTok2-pszMsgTok1);
        bstrValue += _T("\r\n..\r\n");
        pszMsgTok1 = pszMsgTok2 + 5;
      }
    } while (pszMsgTok2);
    
    bstrValue += pszMsgTok1;
    bstrValue += _T("\r\n");

    // Encode the message using the desired encoding method
    switch(MessageEncode)
    {
    case encodeQuotedPrintable:
      bstrMessage += _T("Content-Transfer-Encoding: quoted-printable\r\n\r\n");
      bstrTemp.Attach(EncodeQuotedPrintable(bstrValue));
      break;
    case encodeBase64:
      bstrMessage += _T("Content-Transfer-Encoding: base64\r\n\r\n");
      cvt.Encode(OLE2T(bstrValue));
      bstrValue = cvt.EncodedMessage();
      bstrTemp.Attach(BreakMessage(bstrValue));
      break;
    default: // Default to 7bit encoding when in doubt
      bstrMessage += _T("Content-Transfer-Encoding: 7bit\r\n\r\n");
      bstrTemp.Attach(BreakMessage(bstrValue));
      break;
    }
    // Add the message to the output stream
    bstrMessage += bstrTemp;
    bstrTemp.Empty();
    bstrValue.Empty();
  }

  // Process any attachments
  for (n = 0;n < Attachments.GetSize();n++)
  {
    HANDLE hFile;
    LPBYTE pData;
    DWORD dwSize;
    DWORD dwBytes;
    LPTSTR pszTok;

    // Get the filename of the attachment
    bstrValue = Attachments[n];    

    // Open the file
    hFile = CreateFile(OLE2T(bstrValue),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
      // Get the size of the file, allocate the memory and read the contents.
      dwSize = GetFileSize(hFile,NULL);
      pData = (LPBYTE)malloc(dwSize);
      if (ReadFile(hFile,pData,dwSize,&dwBytes,NULL))
      {
        // BASE64 encode the message
        cvt.Encode(pData,dwBytes);

        // Write out our boundary marker
        if (bstrBoundary.Length())
        {
          bstrMessage += _T("--");
          bstrMessage += bstrBoundary;
          bstrMessage += _T("\r\n");
        }
        
        // Strip the path from the filename
        pszFile = wcsrchr(bstrValue,'\\');
        if (!pszFile) pszFile = bstrValue.m_str;
        else pszFile ++;

        // Write out the content type and attachment types to the message
        bstrMessage += _T("Content-Type: application/octet-stream;\r\n\tname=\"");
        bstrMessage += pszFile;
        bstrMessage += _T("\"\r\n");
        bstrMessage += _T("Content-Transfer-Encoding: base64\r\n");
        bstrMessage += _T("Content-Disposition: attachment;\r\n\tfilename=\"");
        bstrMessage += pszFile;
        bstrMessage += _T("\"\r\n\r\n");

        // Get the encoded message and add it to the message body.
        bstrValue.Empty();
        pszTok = (LPTSTR)cvt.EncodedMessage();
        bstrTemp = pszTok;
        bstrValue = BreakMessage(bstrTemp);
        bstrValue += _T("\r\n");
        bstrMessage += bstrValue;
        bstrTemp.Empty();
        bstrValue.Empty();
      }
      // Close the file and clear the temp buffer
      CloseHandle(hFile);
      free(pData);
    }
  }
  // If we are multipart, write out the trailing end sequence
  if (bstrBoundary.Length())
  {
    bstrMessage += _T("--");
    bstrMessage += bstrBoundary;
    bstrMessage += _T("--\r\n");
  }
  return bstrMessage.Detach();
}

// Parses text into quoted-printable lines.
// See RFC 1521 for full details on how this works.
BSTR CSmtpMessage::EncodeQuotedPrintable(BSTR bstrSrc)
{
  CComBSTR bstrMsg;
  CComBSTR bstrTemp;
  CComBSTR bstrTemp2;
  LPOLESTR pszTok1 = bstrSrc;
  LPOLESTR pszTok2;
  int n;

  if (!bstrSrc) return NULL;

  // Change = signs to =3D
  pszTok1 = bstrSrc;
  do
  {
    pszTok2 = wcschr(pszTok1,'=');
    if (pszTok2)
    {
      bstrTemp.Append(pszTok1,pszTok2-pszTok1);
      bstrTemp += _T("=3D");
      pszTok1 = pszTok2 + 1;
    }
  } while (pszTok2);
  // Append anything left after the search
  if (wcslen(pszTok1)) bstrTemp += pszTok1;

  pszTok1 = wcstok(bstrTemp.m_str,OLESTR("\r\n"));
  while (pszTok1)
  {
    while (1)
    {
      if (wcslen(pszTok1) > 76)
      {
        n = 75; // Breaking at the 75th character
        if (pszTok1[n-1] == '=') n -= 1; // If the last character is an =, don't break the line there
        else if (pszTok1[n-2] == '=') n -= 2; // If we're breaking in the middle of a = sequence, back up!
        
        // Append the first section of the line to the total string
        bstrMsg.Append(pszTok1,n);
        bstrMsg += _T("=\r\n");
        pszTok1 += n;
      }
      else // Line is less than or equal to 76 characters
      {
        n = wcslen(pszTok1); // If we have some trailing data, process it.
        if (n)
        {
          if (pszTok1[n-1] == ' ') // Last character is a space
          {
            // Replace the last character with an =20 sequence
            pszTok1[n-1] = 0;
            bstrTemp2 = pszTok1;
            bstrTemp2 += _T("=20");
            // Since the string may now be larger than 76 characters, we have to reprocess the line
            pszTok1 = bstrTemp2.m_str;
          }
          else // Last character is not a space
          {
            bstrMsg += pszTok1;
            bstrMsg += _T("\r\n");
            break; // Exit the loop which processes this line, and move to the next line
          }
        }
        else break; // Move to the next line
      }
    }
    pszTok1 = wcstok(NULL,OLESTR("\r\n"));
  }
  return bstrMsg.Detach();
}

// Breaks a message's lines into a maximum of 76 characters
// Does some semi-intelligent wordwrapping to ensure the text is broken properly.
// If a line contains no break characters, it is forcibly truncated at the 76th char
BSTR CSmtpMessage::BreakMessage(BSTR bstrSrc, int nLength)
{
  CComBSTR bstrMsg;
  CComBSTR bstrTemp = bstrSrc;
  LPOLESTR pszTok1;
  LPOLESTR pszTok2;
  LPOLESTR pszBreaks = OLESTR(" -;.,?!");

  if (!bstrSrc) return NULL;

  // Process each line one at a time
  pszTok1 = wcstok(bstrTemp.m_str,OLESTR("\r\n"));
  while (pszTok1)
  {
    while (wcslen(pszTok1) > 76)
    {
      // Start at the 76th character, and move backwards until we hit a break character
      pszTok2 = &pszTok1[75];
      
      // If there are no break characters in the string, skip the backward search for them!
      if (wcspbrk(pszTok1,pszBreaks))
      {
        while (!wcschr(pszBreaks,*pszTok2) && pszTok2 > pszTok1)
          pszTok2--;
      }
      // Add the first part of the message to the final output, add a CRLF
      bstrMsg.Append(pszTok1,(pszTok2 - pszTok1) + 1);
      bstrMsg += _T("\r\n");
      // Replace the original 77th character back, and advance to that character.
      pszTok1 = &pszTok2[1];
    }
    bstrMsg += pszTok1;
    bstrMsg += _T("\r\n");
    pszTok1 = wcstok(NULL,OLESTR("\r\n"));
  }
  return bstrMsg.Detach();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtp
//////////////////////////////////////////////////////////////////////
CSmtp::CSmtp()
{
  LPSERVENT pEnt;
  
  m_bExtensions = TRUE;       // Use ESMTP if possible
  m_dwCmdTimeout = 30;        // Default to 30 second timeout
  m_hSocket = INVALID_SOCKET;

  // Try and get the SMTP service entry by name
  pEnt = getservbyname("SMTP","tcp");
  if (pEnt) m_wSmtpPort = pEnt->s_port;
  else m_wSmtpPort = htons(25);

}

CSmtp::~CSmtp()
{
  // Make sure any open connections are shut down
  Close();
}

BOOL CSmtp::Connect(LPTSTR pszServer)
{
  USES_CONVERSION;
  SOCKADDR_IN addr;
  int nRet;

  // Shut down any active connection
  Close();

  // Resolve the hostname
  addr.sin_family = AF_INET;
  addr.sin_port = m_wSmtpPort;
  addr.sin_addr.s_addr = inet_addr(T2A(pszServer));
  if (addr.sin_addr.s_addr == INADDR_NONE)
  {
    LPHOSTENT pHost = gethostbyname(T2A(pszServer));
    if (!pHost) return FALSE;
    addr.sin_addr.s_addr = *(LPDWORD)pHost->h_addr;
  }

  // Create a socket
  m_hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if (m_hSocket == INVALID_SOCKET) return FALSE;

  // Connect to the host
  if (connect(m_hSocket,(LPSOCKADDR)&addr,sizeof(addr)) == SOCKET_ERROR)
  {
    Close();
    return FALSE;
  }

  // Get the initial response string
  nRet = SendCmd(NULL);
  if (nRet != 220)
  {
    RaiseError(nRet);
    Close();
    return FALSE;
  }

  // Send a HELLO message to the SMTP server
  if (SendHello())
  {
    Close();
    return FALSE;
  }

  return TRUE;
}

void CSmtp::Close()
{
  if (m_hSocket != INVALID_SOCKET)
  {
    // If we're connected to a server, tell them we're quitting
    if (m_bConnected) SendQuitCmd();
    // Shutdown and close the socket
    shutdown(m_hSocket,2);
    closesocket(m_hSocket);
  }
  m_hSocket = INVALID_SOCKET;
}

// Send a command to the SMTP server and wait for a response
int CSmtp::SendCmd(LPTSTR pszCmd)
{
  USES_CONVERSION;
  FD_SET set;
  TIMEVAL tv;
  int nRet;
  DWORD dwTick;
  CHAR szResult[CMD_RESPONSE_SIZE];
  LPSTR pszPos;
  LPSTR pszTok;

  // If we have a command to send, then send it.
  if (pszCmd)
  {
    nRet = send(m_hSocket,T2A(pszCmd),lstrlen(pszCmd),0);
    if (nRet == SOCKET_ERROR) return nRet;
  }

  // Get ready to wait for a response
  ZeroMemory(szResult,CMD_RESPONSE_SIZE);
  FD_ZERO(&set);
  pszPos = szResult;

  // Wait for the specified timeout for a full response string
  dwTick = GetTickCount();
  while (GetTickCount() - dwTick < (m_dwCmdTimeout * 1000))
  {
    FD_SET(m_hSocket,&set);
    
    tv.tv_sec = m_dwCmdTimeout - ((GetTickCount() - dwTick) / 1000);
    tv.tv_usec = 0;

    // Check the socket for readability
    nRet = select(1,&set,NULL,NULL,&tv);
    if (nRet == SOCKET_ERROR) break;

    // If the socket has data, read it.
    if (nRet == 1)
    {
      nRet = recv(m_hSocket,pszPos,CMD_RESPONSE_SIZE - (pszPos - szResult),0);
      // Treats a graceful shutdown as an error
      if (nRet == 0) nRet = SOCKET_ERROR;
      if (nRet == SOCKET_ERROR) break;
      
      // Add the data to the total response string & check for a LF
      pszPos += nRet;
      pszTok = strchr(szResult,'\n');
      if (pszTok)
      {
        // Truncate CRLF combination and exit our wait loop
        pszTok --;
        pszTok[0] = 0;
        break;
      }
    }
  }
  // Assign the response string
  m_bstrResult = szResult;

  // Evaluate the numeric response code
  if (nRet && nRet != SOCKET_ERROR)
  {
    szResult[3] = 0;
    nRet = atoi(szResult);
    SmtpCommandResponse(pszCmd, nRet, OLE2T(m_bstrResult));
  }
  else nRet = -1;

  return nRet;
}

// Placeholder function -- overridable
// This function is called when the SMTP server gives us an unexpected error
// The <nError> value is the SMTP server's numeric error response, and the <pszErr>
// is the descriptive error text
//
// <pszErr> may be NULL if the server failed to respond before the timeout!
// <nError> will be -1 if a catastrophic failure occurred.
//
// Return 0, or nError.  The return value is currently ignored.
int CSmtp::SmtpError(int nError, LPTSTR pszErr)
{
#ifdef _DEBUG
  if (pszErr)
  {
    OutputDebugString(_T("SmtpError: "));
    OutputDebugString(pszErr);
    OutputDebugString(_T("\n"));
  }
#endif
  return 0;
}

// Placeholder function -- overridable
// Currently the only warning condition that this class is designed for is
// an authentication failure.  In that case, <nWarning> will be 535,
// which is the RFC error for authentication failure.  If authentication
// fails, you can override this function to prompt the user for a new
// username and password.  Change the <m_bstrUser> and <m_bstrPass> member
// variables and return TRUE to retry authentication.
//
// <pszWarning> may be NULL if the server did not respond in time!
//
// Return FALSE to abort authentication, or TRUE to retry.
int CSmtp::SmtpWarning(int nWarning, LPTSTR pszWarning)
{
#ifdef _DEBUG
  if (pszWarning)
  {
    OutputDebugString(_T("SmtpWarning: "));
    OutputDebugString(pszWarning);
    OutputDebugString(_T("\n"));
  }
#endif
  return 0;
}

// Placeholder function -- overridable
// This is an informational callback only, and provides a means to inform
// the caller as the SMTP session progresses.
// ALWAYS check for NULL values on pszCmd and pszResponse before performing
// any actions!
// <nResponse> will be -1 if a catastrophic failure occurred, but that will
// be raised in the SmtpError() event later on during processing.
void CSmtp::SmtpCommandResponse(LPTSTR pszCmd, int nResponse, LPTSTR pszResponse)
{
#ifdef _DEBUG
  if (pszCmd)
  {
    OutputDebugString(_T("SmtpCommand : "));
    OutputDebugString(pszCmd);
  }
  OutputDebugString(_T("SmtpResponse: "));
  OutputDebugString(pszResponse);
  OutputDebugString(_T("\n"));
#endif
}

// Raises a SmtpError() condition
int CSmtp::RaiseError(int nError)
{
  USES_CONVERSION;

  // If the error code is -1, something catastrophic happened
  // so we're effectively not connected to any SMTP server.
  if (nError == -1) m_bConnected = FALSE;
  return SmtpError(nError,OLE2T(m_bstrResult));
}

// Warnings are recoverable errors that we may be able to continue working with
int CSmtp::RaiseWarning(int nWarning)
{
  USES_CONVERSION;
  return SmtpWarning(nWarning,OLE2T(m_bstrResult));
}

// E-Mail's a message
// Returns 0 if successful, -1 if an internal error occurred, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendMessage(CSmtpMessage &msg)
{
  USES_CONVERSION;

  int nRet;
  int n;

  // Check if we have a sender
  if (!msg.Sender.Address.Length()) return -1;
  
  // Check if we have recipients
  if (!msg.Recipient.Address.Length() && !msg.CC.GetSize()) return -1;

  // Check if we have a message body or attachments
  if (!msg.Message.Length() && !msg.Attachments.GetSize()) return -1;

  // Send the sender's address
  nRet = SendFrom(OLE2T(msg.Sender.Address));
  if (nRet) return nRet;
  
  // If we have a recipient, send it
  if (msg.Recipient.Address.Length())
  {
    nRet = SendTo(OLE2T(msg.Recipient.Address));
    if (nRet) return nRet;
  }

  // If we have any CC's, send those.
  for (n = 0;n < msg.CC.GetSize();n++)
  {
    nRet = SendTo(OLE2T(msg.CC[n].Address));
  }

  // If we have any bcc's, send those.
  for (n = 0;n < msg.BCC.GetSize();n++)
  {
    nRet = SendTo(OLE2T(msg.BCC[n].Address));
  }

  // Send the body of the message
  nRet = SendData(msg);

  return nRet;
}

// Tell the SMTP server we're quitting
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendQuitCmd()
{
  int nRet;

  if (!m_bConnected) return 0;

  nRet = SendCmd(_T("QUIT\r\n"));
  if (nRet != 221) RaiseError(nRet);

  m_bConnected = FALSE;

  return (nRet == 221) ? 0:nRet;
}

// Initiate a conversation with the SMTP server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendHello()
{
  int nRet = 0;
  TCHAR szName[64];
  TCHAR szMsg[MAX_PATH];
  DWORD dwSize = 64;

  GetComputerName(szName,&dwSize);

  // First try a EHLO if we're using ESMTP
  wsprintf(szMsg,_T("EHLO %s\r\n"),szName);
  if (m_bExtensions) nRet = SendCmd(szMsg);

  // If we got a 250 response, we're using ESMTP, otherwise revert to regular SMTP
  if (nRet != 250)
  {
    m_bUsingExtensions = FALSE;
    szMsg[0] = 'H';
    szMsg[1] = 'E';
    nRet = SendCmd(szMsg);
  }
  else m_bUsingExtensions = TRUE;

  // Raise any unexpected responses
  if (nRet != 250)
  {
    RaiseError(nRet);
    return nRet;
  }

  // We're connected!
  m_bConnected = TRUE;

  // Send authentication if we have any.
  // We don't fail just because authentication failed, however.
  if (m_bUsingExtensions) SendAuthentication();

  return 0;
}

// Requests authentication for the session if the server supports it,
// and attempts to submit the user's credentials.
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendAuthentication()
{
  USES_CONVERSION;
  int nRet;
  CBase64 cvt;
  LPCTSTR pszTok;
  TCHAR szMsg[MAX_PATH];

  // This is an authentication loop, we can authenticate multiple times in case of failure.
  while(1)
  {
    // If we don't have a username, skip authentication
    if (!m_bstrUser.Length()) return 0;
    
    // Make the authentication request
    nRet = SendCmd(_T("AUTH LOGIN\r\n"));
    // If it was rejected, we have to abort.
    if (nRet != 334)
    {
      RaiseWarning(nRet);
      return nRet;
    }
    
    // Authentication has 2 stages for username and password.
    // It is possible if the authentication fails here that we can
    // resubmit proper credentials.
    while (1)
    {
      // Decode the authentication string being requested
      cvt.Decode(OLE2T(&m_bstrResult.m_str[4]));
      pszTok = cvt.DecodedMessage();
      
      if (!lstrcmpi(pszTok,"Username:"))
        cvt.Encode(OLE2T(m_bstrUser));
      else if (!lstrcmpi(pszTok,"Password:"))
        cvt.Encode(OLE2T(m_bstrPass));
      else break;
      
      wsprintf(szMsg,_T("%s\r\n"),cvt.EncodedMessage());
      nRet = SendCmd(szMsg);
      
      // If we got a failed authentication request, raise a warning.
      // this gives the owner a change to change the username and password.
      if (nRet == 535)
      {
        // Return FALSE to fail, or TRUE to retry
        nRet = RaiseWarning(nRet);
        if (!nRet)
        {
          // Reset the error back to 535.  It's now an error rather than a warning
          nRet = 535;
          break;
        }
      }
      if (nRet != 334) break;
    }
    if (nRet != TRUE) break;
  }
  // Raise an error if we failed to authenticate
  if (nRet != 235) RaiseError(nRet);

  return (nRet == 235) ? 0:nRet;
}

// Send a MAIL FROM command to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendFrom(LPTSTR pszFrom)
{
  int nRet;
  TCHAR szMsg[MAX_PATH];

  wsprintf(szMsg,_T("MAIL FROM: <%s>\r\n"),pszFrom);
  
  while (1)
  {
    nRet = SendCmd(szMsg);
    // Send authentication if required, and retry the command
    if (nRet == 530) nRet = SendAuthentication();
    else break;
  }
  // Raise an error if we failed
  if (nRet != 250) RaiseError(nRet);
  return (nRet == 250) ? 0:nRet;
}

// Send a RCPT TO command to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendTo(LPTSTR pszTo)
{
  int nRet;
  TCHAR szMsg[MAX_PATH];

  wsprintf(szMsg,_T("RCPT TO: <%s>\r\n"),pszTo);
  
  nRet = SendCmd(szMsg);
  if (nRet != 250 && nRet != 251) RaiseError(nRet);
  return (nRet == 250 || nRet == 251) ? 0:nRet;
}

// Send the body of an e-mail message to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendData(CSmtpMessage &msg)
{
  USES_CONVERSION;
  int nRet;
  CComBSTR bstrMsg;

  // Send the DATA command.  We need a 354 to proceed
  nRet = SendCmd(_T("DATA\r\n"));
  if (nRet != 354)
  {
    RaiseError(nRet);
    return nRet;
  }

  // Parse the body of the email message
  bstrMsg.Attach(msg.Parse());
  bstrMsg += _T("\r\n.\r\n");

  // Send the body and expect a 250 OK reply.
  nRet = SendCmd(OLE2T(bstrMsg));
  if (nRet != 250) RaiseError(nRet);

  return (nRet == 250) ? 0:nRet;
}
