#ifndef H_EMAIL
#define H_EMAIL

#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>

//using namespace Poco;
using namespace Poco::Net;

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
	//std::string CurrentDateTime();
	int SendEmail();
private:
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
	std::string _payload;
	//MailMessage _msg;
};

#endif