
#include <windows.h>
#include <atlbase.h>
#include <stdio.h>
#include "..\smtp.h"

int main(void)
	{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);

	CSmtp mail;
	CSmtpMessage msg;
	CSmtpAddress addr;
	CSmtpMessageBody body;
	CSmtpAttachment attach;

	msg.Sender.Address = "claudio@muconsulting.com";

	addr.Name = "";
	addr.Address = "mailtest1@muconsulting.com";
	msg.Recipients.Add(addr);

	addr.Name = "Claudio at Yahoo";
	addr.Address = "claudiofahey@yahoo.com";
	msg.CC.Add(addr);

#if 0
	addr.Name = "";
	addr.Address = "someuser@muconsulting.com";
	msg.BCC.Add(addr);
#endif
	//msg.Recipient.Address = "someuser@muconsulting.com";

	msg.Subject = "Test1";

	body = "This is a test.";
	msg.Message.Add(body);

	attach = "d:\\muconsulting\\seriallogger\\senddata1.txt";
	//attach = "d:\\muconsulting\\seriallogger\\senddata3.txt";
	//attach = "D:\\Program Files\\Serial Logger Service\\SerialLogger20020408.log";
	//attach = "d:\\ntDETect.cOm";
	//attach = "d:\\dl\\movie2.mpeg";
	//attach.TransferEncoding = encodeBase64;
    msg.Attachments.Add(attach);

	attach = "d:\\dl\\smtpdemo.zip";
	//attach = "D:\\Program Files\\Serial Logger Service\\SerialLogger20020408.log";
    msg.Attachments.Add(attach);

	if (mail.Connect("smtp.socal.rr.com"))
		{
		mail.SendMessage(msg);
		mail.Close();
		}

	WSACleanup();

	return 0;
	}
