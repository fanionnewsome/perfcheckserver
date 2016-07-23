#include "stdafx.h"
#include "PocoEmail.h"


PocoEmail::PocoEmail(std::string sender,
	std::string senderRealName,
	std::string ccRealName,
	std::string bccRealName,
	std::string recipient,
	std::string cc,
	std::string bcc,
	std::string subject,
	std::string body): _sender(sender), _senderRealName(senderRealName),
	    _ccRealName(ccRealName),
	    _bccRealName(bccRealName),
	    _recipient(recipient),
	    _cc(cc),
	    _bcc(bcc),
	    _subject(subject),
	    _body(body)
{
}

int PocoEmail::SendEmail() {
	string host = "smtp.gmail.com";
	UInt16 port = 587;
	string username = "fanionnewsome@gmail.com";
	string password = "IamElitehack0r!";
	string to = "da.perk@gmail.com";
	string from = "<noreply@gmail.com>";
	string subject = "This is a test e-mail message sent using Poco Libraries";
	subject = MailMessage::encodeWord(subject, "UTF-8");
	string content = "Well done! You've successfully sent a message using Poco SMTPClientSession";
	MailMessage message;
	

	message.setSender(from);
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
	message.setSubject(subject);
	message.setContentType("text/plain; charset=UTF-8");
	message.setContent(content, MailMessage::ENCODING_8BIT);
	try {
		SecureSMTPClientSession session(host, port);
		SharedPtr<InvalidCertificateHandler> certificate = new AcceptCertificateHandler(false);
		Context::Ptr context = new Poco::Net::Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		SSLManager::instance().initializeClient(0, certificate, context);

		session.open();
		Poco::Net::initializeSSL();

		try {
			session.login();
			if (session.startTLS(context)) {
				session.login(SMTPClientSession::AUTH_LOGIN, username, password);
				session.sendMessage(message);
			}
			cout << "Message successfully sent" << endl;
			session.close();
			Poco::Net::uninitializeSSL();
		}
		catch (SMTPException &e) {
			cerr << e.displayText() << endl;
			session.close();
			Poco::Net::uninitializeSSL();
			return 0;
		}
	}
	catch (NetException &e) {
		cerr << e.displayText() << endl;
		return 0;
	}
	return 0;
}


PocoEmail::~PocoEmail()
{
}
