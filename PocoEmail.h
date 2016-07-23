#pragma once
#include "Email.h"
class PocoEmail :
	public Email
{
public:
	std::string _sender;
	std::string _senderRealName;
	std::string _ccRealName;
	std::string _bccRealName;
	std::string _recipient;
	std::string _cc;
	std::string _bcc;
	std::string _subject;
	std::string _body;

	PocoEmail(std::string sender,
		std::string senderRealName,
		std::string ccRealName,
		std::string bccRealName,
		std::string recipient,
		std::string cc,
		std::string bcc,
		std::string subject,
		std::string body);
	int SendEmail();
	virtual ~PocoEmail();
};

