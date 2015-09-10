#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>
#include <QDateTime>

#include "base_settings.h"


BaseSettings::BaseSettings()
{
	// файл настроек
	createFilesIniLog();
	writeFileLog("Программа запущена");

	// инициализация настроек
	initSerialPortSettings();
	initDeviceSettings();
}

void BaseSettings::createFilesIniLog()
{
	QString fname;
	fname = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
	fname = fname.left(fname.indexOf('.'));
	this->file_ini.append(fname).append (".ini");
	this->file_log.append(fname).append (".log");
}


void BaseSettings::writeFileLog(QString str)
{
	QDateTime datetime = QDateTime::currentDateTime();
	QFile file(this->file_log);
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return;

	QTextStream out(&file);
	file.write(QString("%1 %2\n").arg(datetime.toString("dd/MM/yyyy hh:mm:ss")).arg(str).toUtf8());

	file.close();
}

QString BaseSettings::getNameFileIni()const
{
	return this->file_ini;
}
QString BaseSettings::getNameFileLog()const
{
	return this->file_log;
}

void BaseSettings::initSerialPortSettings()
{
//	QSettings settings(this->file_ini,QSettings::IniFormat);
//	//	if(QFile::exists(this->file_ini))
//	settings.beginGroup("serial");
//	if(settings.contains("baud_rate"))
//	{
//		qDebug ()<< "port_name" << serial.port_name;

//		serial.port_name    = settings.value("port_name").toString();
//		serial.baud_rate    = settings.value("baud_rate").toInt();
//		serial.word_length  = settings.value("word_length").toInt();
//		serial.stop_bits    = settings.value("stop_bits").toInt();
//		serial.parity       = settings.value("parity").toInt();
//		serial.flow_control = settings.value("flow_control").toInt();
//		settings.endGroup();
//	}
//	else
//	{
//		serial.baud_rate    = 9600;
//		serial.word_length  = 8;
//		serial.stop_bits    = 1;
//		serial.parity       = 0;
//		serial.flow_control = 0;
//	}
}

void BaseSettings::initDeviceSettings()
{
//	QSettings settings(this->file_ini,QSettings::IniFormat);
//	settings.beginGroup("device");
////	if(QFile::exists(this->file_ini))
//	if(settings.contains("modbus_id"))
//	{
//		this->device.update        = settings.value("update").toInt();
//		this->device.modbus_enable = settings.value("modbus_enable").toInt();
//		this->device.modbus_id     = settings.value("modbus_id").toInt ();
//		this->device.device_id     = settings.value("device_id").toInt();
//		settings.endGroup();
////		qDebug() <<  "dev.slave_id" << this->getDeviceSettings().modbus_id;
//	}
//	else
//	{
//		this->device.DATE		= 1;
//		this->device.TIME		= false;
//		this->device.update     = false;
//	}
}

BaseSettings::BASE_SERIAL_PORT_S BaseSettings::getSerialPortSettings()
{
	return this->serial;
}

void BaseSettings::updateSerialPortSettings(BASE_SERIAL_PORT_S serial)
{
	QSettings set(this->getNameFileIni(),QSettings::IniFormat);
	set.beginGroup("serial");
	set.setValue( "port_name", serial.port_name );
	set.setValue( "baud_rate", serial.baud_rate);
	set.setValue( "word_length", serial.word_length );
	set.setValue( "stop_bits", serial.stop_bits );
	set.setValue( "parity", serial.parity );
	set.setValue( "flow_control", serial.flow_control );
	set.endGroup();
	this->serial = serial;
}

BaseSettings::BASE_DEVICE_SETTINGS_S BaseSettings::getDeviceSettings()
{

	return this->device;
}

void BaseSettings::updateDeviceSettings(BASE_DEVICE_SETTINGS_S device)
{
	QSettings set(this->getNameFileIni(),QSettings::IniFormat);
	set.beginGroup("device");
	set.setValue( "state",    device.STATE);
	set.setValue( "update",   device.UPDATE);
	set.setValue( "new",      device.NEW);
	set.setValue( "enable",   device.ENABLE);
	set.setValue( "version",  device.VERSION);
	set.setValue( "date",     device.DATE.Date);
	set.setValue( "month",    device.DATE.Month);
	set.setValue( "year",     device.DATE.Year);
	set.setValue( "hours",    device.TIME.Hours);
	set.setValue( "minutes",  device.TIME.Minutes);
	set.setValue( "seconds",  device.TIME.Seconds);
	set.setValue( "count",    device.COUNT);
	set.endGroup();
	this->device = device;
}

