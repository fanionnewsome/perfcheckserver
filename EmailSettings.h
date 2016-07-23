#ifndef H_EMAIL_SETTINGS
#define H_EMAIL_SETTINGS

#include "Settings.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <rapidxml.hpp>

using namespace rapidxml;
using namespace std;

class EmailSettings :
	public Settings
{
public:
	EmailSettings();
	~EmailSettings();

	virtual void ReadXML();

	std::string getHostname() const { return _hostname; }

	std::string getUsername() const { return _username; }

	std::string getEmailTo() const { return _emailTo; }

	std::string getEmailFrom() const { return _emailFrom; }

	std::string getEmailSubject() const { return _subject;  }

	std::string getEmailContent() const { return _content;  }
	
	unsigned int getPort() const { return _port;  }

	void setHostName(std::string hostname) { _hostname = hostname; }

	void setUsername(std::string username) { _username = username; }

	void setEmailContent(std::string content) { _content = content;  }

	void setEmailTo(std::string emailTo) { _emailTo = emailTo;  }

	void setEmailFrom(std::string emailFrom) { _emailFrom = emailFrom;  }

	void setEmailSubject(std::string emailSubject) { _subject = emailSubject; }
private:
	std::string _hostname;
	std::string _username;
	std::string _password;
	std::string _emailTo;
	std::string _emailFrom;
	std::string _subject;
	std::string _content;
	unsigned int _port;
};

#endif

