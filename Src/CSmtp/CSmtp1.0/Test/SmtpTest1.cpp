
#include <windows.h>
#include <atlbase.h>
#include <stdio.h>
#include "..\smtp.h"

int main(void)
	{
	HRESULT hRes = CoInitialize(NULL);
	_ASSERTE(SUCCEEDED(hRes));

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);

	CSmtp mail;
	CSmtpMessage msg;
	CSmtpAddress addr;

	msg.Sender.Address = "claudio@muconsulting.com";

	addr.Address = "someuser@muconsulting.com";
	msg.Recipient.Add(addr);

	msg.Subject = "Test1";
	msg.Message = "This is a test.";

	msg.Attachments.Add(CComBSTR("d:\\muconsulting\\seriallogger\\senddata3.txt"));
	//msg.Attachments.Add(CComBSTR("D:\\Program Files\\Serial Logger Service\\SerialLogger20020408.log"));

	if (mail.Connect("smtp.socal.rr.com"))
		{
		mail.SendMessage(msg);
		mail.Close();
		}

	WSACleanup();

	return 0;
	}
