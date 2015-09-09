/*
  Copyright (c) 2011 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  SmtpClient for Qt is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  SmtpClient for Qt is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the LICENCE file for more details.
*/

#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include "SmtpMime"

#include "settings.h"


namespace Ui {
 //   class Settings;
    class SettingsEmail;
}

class SendEmail : public QObject
{
    Q_OBJECT

public:
	 SendEmail(Settings::EMAIL_SETTINGS_T *email);
    ~SendEmail();

    static EmailAddress * stringToEmail(const QString & str);

public slots:
    void sendEmail(Settings::EMAIL_SETTINGS_T *email);

private:
};

#endif // SENDEMAIL_H
