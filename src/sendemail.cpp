/*
  Copyright (c) 2011 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  SmtpClient for Qt is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  SmtpClient for Qt is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the LICENSE file for more details.
*/


#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "sendemail.h"


using namespace std;

SendEmail::SendEmail(Settings::EMAIL_SETTINGS_T *email)
{
}

SendEmail::~SendEmail()
{
   // delete ui;
}

EmailAddress* SendEmail::stringToEmail(const QString &str)
{
	int p1 = str.indexOf("<");
	int p2 = str.indexOf(">");

	if (p1 == -1)
	{
		// no name, only email address
		return new EmailAddress(str);
	}
	else
	{
		return new EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
	}

}

void SendEmail::sendEmail(Settings::EMAIL_SETTINGS_T *email)
{
	email->enable = false;
	email->error.clear();

	SmtpClient smtp (email->host,email->port, email->ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

	MimeMessage message;

	QStringList rcptStringList = email->recipients.split(';');

	message.setSender(stringToEmail(email->sender));
	message.setSubject(email->subject);

	for (int i = 0; i < rcptStringList.size(); ++i)
		 message.addRecipient(stringToEmail(rcptStringList.at(i)));

	MimeHtml content;
	content.setHtml(email->texteditor);

	message.addPart(&content);
	for (int i = 0; i < email->attachments.count(); ++i)
	{
		message.addPart(new MimeAttachment(new QFile(email->attachments[i])));
	}

	if (!smtp.connectToHost())
	{
		email->error.append("Connection Failed");
		return;
	}

	if (email->auth)
		if (!smtp.login(email->username, email->password))
		{
			email->error.append("Authentification Failed");
			return;
		}

	if (!smtp.sendMail(message))
	{
		email->error.append("Mail sending Failed");
		return;
	}
	else
	{
		email->enable = true;
		email->error.append("The email was succesfully sended.");
	}
	smtp.quit();
	return;
}
