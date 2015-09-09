#include <QCoreApplication>
//#include <QFile>
//#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>
#include <QDateTime>

#include "base_settings.h"
#include "settings.h"


Settings::Settings()
{
	// файл настроек
//	createNameIniLog();
	//createJournalLog();
//	BaseSettings::getInstance().writeFileLog(codec->toUnicode("Программа запущена"));

	// инициализация настроек
//	initSerialPortSettings();
//	initDeviceSettings();
	initEmailSettings();
    initProgramSettings();
//    initResetSettings();
//	initAlarmSettings();
}


void Settings::initEmailSettings()
{
    BaseSettings  &  basesettings = BaseSettings::getInstance();
	QSettings settings(basesettings.getNameFileIni(),QSettings::IniFormat);
	settings.beginGroup("email");
    if(settings.contains("host"))
    {
        this->email.enable     = settings.value("enable").toBool();
        this->email.error      = settings.value("error").toString();
        this->email.host       = settings.value("host").toString();
        this->email.port       = settings.value("port").toInt();
        this->email.ssl        = settings.value("ssl").toBool();
        this->email.auth       = settings.value("auth").toBool();

        this->email.username   = settings.value("username").toString();
        this->email.password   = settings.value("password").toString();

        this->email.sender     = settings.value("sender").toString();
        this->email.recipients = settings.value("recipients").toString();

        this->email.subject    = settings.value("subject").toString();
        this->email.texteditor = settings.value("texteditor").toString();

        this->email.attachments = settings.value("attachments").toStringList();
        settings.endGroup();
        qDebug () << this->email.attachments;
    }
    else
	{
#ifdef __DEBUG__
//		this->email.enable     = false;
//		this->email.error      = "The email was succesfully sended.";
//		this->email.host       = "smtp.mastermail.ru";
//		this->email.port       = 465;
//		this->email.ssl        = true;
//		this->email.auth       = true;

//		this->email.username   = "sosnin@sapfir.biz";
//		this->email.password   = "cjcyby1963";

//		this->email.sender     = "sosnin@sapfir.biz";
//		this->email.recipients = "sosnin1963@gmail.com;sosnin@sapfir.biz";

//		this->email.subject    = QString("тест");
//		this->email.texteditor = QString("тест");

#endif
	}
}

void Settings::initProgramSettings()
{
    BaseSettings  &  basesettings = BaseSettings::getInstance();
	QSettings settings(basesettings.getNameFileIni(),QSettings::IniFormat);
	settings.beginGroup("program");

	if(settings.contains("auto_load"))
	{
		this->program.auto_load             = settings.value("auto_load").toBool();
		this->program.journal_email_enable  = settings.value("journal_email_enable").toBool();
		this->program.journal_email_timeout = settings.value("journal_email_timeout").toInt();
		this->program.alarm_email_enable    = settings.value("alarm_email_enable").toBool();
		this->program.alarm_email_timeout   = settings.value("alarm_email_timeout").toInt();
//		this->program.dust_threshold        = settings.value("dust_threshold").toInt();
//		this->program.ow_threshold          = settings.value("ow_threshold").toInt();
		this->program.tray_enable           = settings.value("tray_enable").toBool();
		this->program.tray_timeout          = settings.value("tray_timeout").toInt();
		this->program.door_state            = settings.value("door_state").toBool();
		settings.endGroup();
	}
	else
	{
		this->program.auto_load             = false;		// автозагрузка
		this->program.alarm_email_enable    = false;	//
		this->program.alarm_email_timeout   = 10;		 //мин
		this->program.journal_email_enable  = false;	// флаг высылки почты
		this->program.journal_email_timeout = 24;		// периодическая высылка почты час
//		this->program.dust_threshold        = 70;		// порог запыленности
//		this->program.ow_threshold          = 60;		// порог температуры
		this->program.tray_enable           = true;		// вывод в трей
		this->program.tray_timeout          = 10;		// периодическое напоминание в трее мин
	}
}

//void Settings::initAlarmSettings()
//{
//	this->alarm.door   = false;
//	this->alarm.temper = false;
//	this->alarm.dust   = false;
//	this->alarm.tray_timeout   = 2;

//}

Settings::EMAIL_SETTINGS_T Settings::getEmailSettings()
{
	return this->email;
}
void Settings::updateEmailSettings(EMAIL_SETTINGS_T email)
{
	QSettings settings(BaseSettings::getInstance().getNameFileIni(),QSettings::IniFormat);
	settings.beginGroup("email");
	settings.setValue( "enable",     email.enable );
	settings.setValue( "error",      email.error );
	settings.setValue( "host",       email.host );
	settings.setValue( "port",       email.port );
	settings.setValue( "ssl",        email.ssl);
	settings.setValue( "auth",       email.auth );
	settings.setValue( "username",   email.username );
	settings.setValue( "password",   email.password );
	settings.setValue( "sender",     email.sender );
	settings.setValue( "recipients", email.recipients );
	settings.setValue( "subject",    email.subject );
	settings.setValue( "texteditor", email.texteditor );
	settings.setValue( "attachments",QStringList ( email.attachments ));
	settings.endGroup();

	this->email = email;
}

Settings::PROGRAM_SETTINGS_T Settings::getProgramSettings()
{
	return this->program;
}

void Settings::updateProgramSettings(PROGRAM_SETTINGS_T program)
{
	QSettings settings(BaseSettings::getInstance().getNameFileIni(),QSettings::IniFormat);
	settings.beginGroup("program");
	settings.setValue( "auto_load", program.auto_load );

	settings.setValue( "journal_email_enable",  program.journal_email_enable );
	settings.setValue( "journal_email_timeout", program.journal_email_timeout );

	settings.setValue( "alarm_email_enable",    program.alarm_email_enable);
	settings.setValue( "alarm_email_timeout",   program.alarm_email_timeout );

//	settings.setValue( "dust_threshold",        program.dust_threshold );
//	settings.setValue( "ow_threshold",          program.ow_threshold );

	settings.setValue( "tray_enable",           program.tray_enable );
	settings.setValue( "tray_timeout",          program.tray_timeout );

	settings.setValue( "door_state",            program.door_state );
	settings.endGroup();


	this->program = program;
}


//void Settings::initResetSettings()
//{
//    BaseSettings  &  basesettings = BaseSettings::getInstance();
//    QSettings settings(basesettings.getNameFileIni(),QSettings::IniFormat);
//    settings.beginGroup("reset");

//    if(settings.contains("active"))
//    {
//        this->reset.active     = settings.value("active").toBool();
//        this->reset.time_wait  = settings.value("time_wait").toInt();
//        this->reset.time_press = settings.value("time_press").toInt();
//        settings.endGroup();
//    }
//    else
//    {
//        this->reset.active     = false;		//
//        this->reset.time_wait  = 60;		 //мин
//        this->reset.time_press = 2;	//
//    }
//}
//Settings::RESET_SETTINGS_T Settings::getResetSettings()
//{
//	return this->reset;
//}

//void Settings::updateResetSettings(RESET_SETTINGS_T reset)
//{
//	QSettings settings(BaseSettings::getInstance().getNameFileIni(),QSettings::IniFormat);
//	settings.beginGroup("reset");
//	settings.setValue  ("active",     reset.active );
//	settings.setValue  ("time_wait",  reset.time_wait );
//	settings.setValue  ("time_press", reset.time_press );
////	settings.setValue  ("time_press", reset.time_last );
//	settings.endGroup();
//	this->reset = reset;
//}

