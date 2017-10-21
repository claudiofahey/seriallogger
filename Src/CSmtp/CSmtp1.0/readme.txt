CSmtp -- A moderatly simple SMTP message sending class using ATL
Written by Robert Simpson (robert@blackcastlesoft.com)
Version 1.0
Released to the public domain 11/2/2000

WARNING!
This code was written in about 16hrs, and has only been tested for basic
functionality under optimal circumstances.
USE AT YOUR OWN RISK!

The CSmtp class and support classes (CSmtpMessage and CSmtpAddress) are
intended to be as basic of a SMTP mailer as possible while still
providing support for a reasonable amount of features, including:

- Optional SMTP authentication (username/password)
- Transfer-encoding of message text using 7bit ASCII, quoted-printable
  and base64.
- Supports multiple recipients using CC and BCC fields
- Multipart MIME support
- Supports any number of file attachments, which are all encoded as
  application/octet-stream, base64.
- CSmtp is overridable and has several event mechanisms to notify the
  parent of progress during transactions with the server.

The CBase64 class is a renamed version of the Base64Coder demo code found
on the Microsoft website.  Look for Q191239 for more information.

The remainder of the code is all mine, so please report and bugs!

HOW TO USE -- 10 EASY STEPS
   1.  Initialize Winsock
   2.  Create a CSmtp class and a CSmtpMessage class
   3.  Assign values to the Sender, Recipient, Subject and Message members
       of the CSmtpMessage object.
   4.  If sending attachments, add the full filename/path to the Attachments
       array of CSmtpMessage
   5.  If sending to multiple recipients, create a CSmtpAddress, assign the
       Name and Address, then add the address to the CC or BCC members of
       CSmtpMessage.
   6.  If the SMTP server requires authentication, assign the m_bstrUser and
       m_bstrPass members of CSmtp.
   7.  Using the CSmtp class, Connect() to a SMTP server
   8.  Call SendMessage() on the CSmtp class to send the mail.
   9.  Close() the session.
  10.  Cleanup Winsock.

For sample code, check the smtpapp.cpp file

IMPROVEMENTS YOU CAN MAKE
These are features you can add to the code to make it more robust, or suit a
wider variety of applications.
 CSmtpMessage
  - Replace the Message member with a map of CComBSTRs.  The Key value is the
    content type, and the Value is the message content.  Modify the Parse()
    routine to check how many items are in the map to see if the message is
    multipart, and modify the message part of the parsing to parse the map
    based on the content type key.  This will allow you to include a
    multipart text message, containing a rich-text, plain-text and html body.

  - Make attachments smarter by checking the file extension against the
    registry to see if there's a content-type key.  You can then encode 
    attachments in either a base64 stream or quoted-printable if it is text.
    This is done in the Parse() routine.

 CSmtp
  - Support asynchronous processing.  Make the CSmtp class create a separate
    thread to do its processing in.  Add an AsyncConnect(), AsyncClose() and
    AsyncSendMessage() to the class.

  - Support a wider variety of authentication schemes.  Way beyond my scope :)

---
Robert Simpson
Programmer at Large
Black Castle Software, LLC
http://www.blackcastlesoft.com
mailto:robert@blackcastlesoft.com
