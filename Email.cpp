#include "stdafx.h"
#include "Email.h"


Email::Email(
	std::string sender,
	std::string senderRealName,
	std::string ccRealName,
	std::string bccRealName,
	std::string recipient,
	std::string cc,
	std::string bcc,
	std::string subject,
	std::string body) :
	_sender(sender),
	_senderRealName(senderRealName),
	_ccRealName(ccRealName),
	_bccRealName(bccRealName),
	_recipient(recipient),
	_cc(cc),
	_bcc(bcc),
	_subject(subject),
	_body(body)
{
	

}

int Email::SendEmail() {

	

		/*std::stringstream ss;
		std::string hostname("smtp.gmail.com");
		UINT16 smtpPort = 25;
		std::string username = "pistolpete78";
		std::string password = "newg4y2k";

		MailMessage _msg;

		if (_recipient.size() > 0) {
			_msg.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT,
				_recipient, _senderRealName));
		}

		if (_cc.size() > 0) {
			_msg.addRecipient(MailRecipient(MailRecipient::CC_RECIPIENT, _cc, _ccRealName));
		}

		if (_bcc.size() > 0) {
			_msg.addRecipient(MailRecipient(MailRecipient::BCC_RECIPIENT, _bcc, _bccRealName));
		}

		std::string sender("");
		ss << _senderRealName << " " << "<" << _sender << ">";
		sender = ss.str();
		_msg.setSender(sender);

		std::string encodedSubject = MailMessage::encodeWord(_subject, "UTF-8");
		_msg.setSubject(encodedSubject);
		_msg.setContentType("text/plain; charset=UTF-8");
		_msg.setContent(_body, MailMessage::ENCODING_8BIT);
		try
		{
			SMTPClientSession connection(hostname, smtpPort);
			connection.open();
			try
			{
				connection.login(SMTPClientSession::AUTH_LOGIN, username, password);
				connection.sendMessage(_msg);
				std::cout << "Sent mail successfully!" << std::endl;
				connection.close();
			}
			catch (SMTPException &ex) {
				std::cerr << "failed to send mail: " << ex.displayText() << std::endl;
				return EXIT_FAILURE;
			}
		}
		catch (NetException &ex) {
			std::cerr << ex.displayText() << std::endl;
			return EXIT_FAILURE;
		}*/
		return 0;
}

/*std::string Email::CurrentDateTime() {
	time_t rawTime;
	struct tm * timeInfo;
	char buffer[80];

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeInfo);
	std::string dateTime(buffer);

	return dateTime;
}*/

Email::~Email()
{
}
