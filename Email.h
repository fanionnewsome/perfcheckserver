#ifndef H_EMAIL
#define H_EMAIL

#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <Poco/DateTime.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/POP3ClientSession.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SecureSMTPClientSession.h>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

class Email
{
public:

	Email(
		std::string sender, 
		std::string senderRealName,
		std::string ccRealName,
		std::string bccRealName,
		std::string recipient, 
		std::string cc,
		std::string bcc,
		std::string subject, 
		std::string body);
	~Email();

	// Methods
	virtual int SendEmail() = 0;

	Email();

	std::string _sender;
	std::string _senderRealName;
	std::string _ccRealName;
	std::string _bccRealName;
	std::string _recipient;
	std::string _cc;
	std::string _bcc;
	std::string _subject;
	std::string _body;

};

#endif