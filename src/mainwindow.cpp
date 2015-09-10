/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDateTime>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "console.h"
#include "settingsdialog.h"

#include "base_settings.h"
#include "device_code.h"
#include "settings.h"
#include "settingsemail.h"
#include "settingsprogram.h"
#include "settingsupdate.h"
#include "frmmain.h"
#include "frmabout.h"
#include "sendemail.h"
#include "updatedevice.h"


QT_USE_NAMESPACE

#define WAIT_DELAY 650

//! [0]
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle(qApp->applicationName());

	// Отключаем изменение размера формы:
	this->setFixedSize(this->size());

	// Отключаем все кнопки, кроме системного меню и кнопки закрытия:
	this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
						 Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
						 Qt::WindowCloseButtonHint | Qt::Dialog);

	/* устанавливаем флаг чтобы не закрывалась программа после выхода из меню */
	QApplication::setQuitOnLastWindowClosed(false);
	this->createTray();

	fileDialog = new QFileDialog;


	ui->tab->hide();
	ui->tab_2->show();
	ui->tab_3->hide();
	ui->tabWidget->tabBar()->hide();

	ui->btnOpenReset->setDisabled(true);


	serial = new QSerialPort(this);
	settings = new SettingsDialog;

	timer_polling = new QTimer(this);
	QObject::connect(timer_polling, SIGNAL(timeout()), SLOT(slotPollingDevice()));
	timer_polling->setInterval(1000);
	timer_polling->start();

	connect(ui->btnOpenReset, SIGNAL(clicked()), SLOT(slotResetDoorEvent()));

	connect(ui->btnStartProg, SIGNAL(clicked()), SLOT(slotStartProg()));
	connect(ui->btnFileOpen,  SIGNAL(clicked()), fileDialog, SLOT(exec()));
	connect(fileDialog,SIGNAL(fileSelected(QString)),ui->filePath,SLOT(setText(QString)));


}
//! [3]

MainWindow::~MainWindow()
{
	delete settings;
	delete ui;
}

/*
 * отправка данных
 */
void MainWindow::slotPollingDevice()
{
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
	Settings::PROGRAM_SETTINGS_T program = Settings::getInstance().getProgramSettings();

//qDebug() << "slotPollingDevice" ;
	if(device.STATE == BaseSettings::IDLE)
	{
		device.STATE  = BaseSettings::SEARCH;
		device.UPDATE = true;
		/* обновим значения устройства */
		BaseSettings::getInstance().updateDeviceSettings(device);
	}
	if(device.STATE == BaseSettings::SEARCH)
	{
		static char cnt=0;
		if (cnt >=QSerialPortInfo::availablePorts().size())
			cnt = 0;
		QList<QSerialPortInfo>info = QSerialPortInfo::availablePorts();
		QString name = info.takeAt(cnt).portName();

		cnt++;
		qDebug()<< name;
		SettingsDialog::Settings p = settings->settings();
		serial->setPortName(name);
		serial->setBaudRate(QSerialPort::Baud115200 /*p.baudRate*/);
		serial->setDataBits(p.dataBits);
		serial->setParity(p.parity);
		serial->setStopBits(p.stopBits);
		serial->setFlowControl(p.flowControl);
		sendCommand(READ_ID);
	}
	if(device.STATE == BaseSettings::READY)
	{
		if(device.NEW)
		{
			// запишем текущее время в устройство
			QDate date = QDate::currentDate();
			QTime time = QTime::currentTime();
			settingsPacket.data[0] = (char) (date.year() -2000);
			settingsPacket.data[1] = (char) (date.month());
			settingsPacket.data[2] = (char) (date.day());
			settingsPacket.data[3] = (char) (time.hour());
			settingsPacket.data[4] = (char) (time.minute());
			settingsPacket.data[5] = (char) (time.second());
			sendCommand(SET_TIME);
		}
		else
			sendCommand(GET_DOOR_EVENT);
	}
//	if(device.STATE == BaseSettings::UPDATE)
//	{
//		 ui->progressBar->setValue((dataCounter*100)/bufLenght);
//		return;
//	}
	if(program.alarm_email_enable && device.COUNT)
	{
		QDateTime datetime = QDateTime::currentDateTime();
		static QDateTime datetimeemail = datetime ;

		/* если значение изменилось сбрасываем отчет времени */
		if(device.UPDATE)
			datetimeemail = datetime ;

		if( datetime >= datetimeemail)
		{
			datetimeemail = datetime.addSecs(60 * program.alarm_email_timeout);
			handlerEmail();
		}
	}

	if(device.UPDATE)
	{
		BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
		 /* обновим требумые значения если произошло событие */
		handlerDisplay();
		handlerTray();

		device.UPDATE = false;
		/* обновим значения устройства */
		BaseSettings::getInstance().updateDeviceSettings(device);
	}

}


/*
 * сброс аварийного сообщения
 */
void MainWindow::slotResetDoorEvent()
{
	sendCommand(RESET_DOOR_EVENT);
	this->hide();
}

/*
 *  Отправка команд устройству
 */
void MainWindow::sendCommand(Commands command, const void* data)
{
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
//	qDebug() << "sendCommand" ;
	SettingsDialog::Settings p = settings->settings();
	if(device.STATE != BaseSettings::FIRMWARE)
	{

		if (serial->open(QIODevice::ReadWrite))
		{
			timer_polling->stop();
			settingsPacket.startByte=0xAB;
			settingsPacket.command=command;
			//	serial->flush();
			if (data != NULL)
				memcpy(settingsPacket.data, data, sizeof(SettingsPacket));
			serial->write(reinterpret_cast<char*>(&settingsPacket));
			if(serial->waitForReadyRead(WAIT_DELAY))
			{
				QByteArray responseData = serial->readAll();
				while (serial->waitForReadyRead(10))
					responseData += serial->readAll();

				memcpy(&settingsPacket,responseData.data(),sizeof(settingsPacket));

				parseResponsePacket();
				//			ui->statusBar->showMessage("serial->readAll(");

			}
			else
			{
				//			ui->statusBar->showMessage("timeout");
			}
			serial->close();
			//	if(device.STATE != BaseSettings::FIRMWARE)
			{
				/* запускаем таймер опроса устройства */
				timer_polling->start(1000);
			}
		}
		else
		{
			//		ui->statusBar->showMessage(tr("Open error"));
			BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
			device.ENABLE = false;
			device.STATE  = BaseSettings::IDLE;
			BaseSettings::getInstance().updateDeviceSettings(device);
		}
	}
	else
	{
		timer_polling->stop();
		settingsPacket.startByte = 0xAB;
		settingsPacket.command   = command;
		//	serial->flush();
		if (data != NULL)
			memcpy(settingsPacket.data, data, sizeof(SettingsPacket));
		serial->write(reinterpret_cast<char*>(&settingsPacket));
		if(serial->waitForReadyRead(WAIT_DELAY))
		{
			QByteArray responseData = serial->readAll();
			while (serial->waitForReadyRead(10))
				responseData += serial->readAll();

			memcpy(&settingsPacket,responseData.data(),sizeof(settingsPacket));

			parseResponsePacket();
			//			ui->statusBar->showMessage("serial->readAll(");

		}

	}
}



unsigned char* sendData;

/*
 * обработка полученного пакета
 */
void MainWindow::parseResponsePacket()
{
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
	//qDebug() << "parseRecivePacket" ;
	static quint32 _count = 0;

	if(settingsPacket.startByte==0xAB)
	{
		/* обработка команд обмена */
		if(settingsPacket.data[0]==1)
		{
			switch (settingsPacket.command)
			{
				case READ_ID:
					qDebug() << "READ_ID" ;
					device.STATE = BaseSettings::READY;
					device.ENABLE = true;
					device.NEW    = true;
					device.UPDATE = true;

					/* запоминаем версию */
					strcpy(device.VERSION,(char*)(settingsPacket.data+1));

					/* обновим значения устройства */
					BaseSettings::getInstance().updateDeviceSettings(device);

					break;

				case SET_TIME:
					qDebug() << "SET_TIME" ;
//					console->putData("SET_TIME");
					device.NEW    = false;
			//		device.UPDATE = true;
					/* обновим значения устройства */
					BaseSettings::getInstance().updateDeviceSettings(device);
					break;

				case GET_DOOR_EVENT:
//qDebug() << "GET_DOOR_EVENT -DOOR_OPEN -COUNT-" << settingsPacket.data[1] << settingsPacket.data[2];
					if((device.DOOR_OPEN != settingsPacket.data[1]) || (_count != settingsPacket.data[2]) )
					{
						qDebug()<< "GET_DOOR_EVENT";
						_count              = settingsPacket.data[2];
						device.UPDATE       = true;
						device.DOOR_OPEN    = settingsPacket.data[1];
						device.COUNT        = settingsPacket.data[2];
						device.TIME.Hours   = settingsPacket.data[3];
						device.TIME.Minutes = settingsPacket.data[4];
						device.TIME.Seconds = settingsPacket.data[5];
						device.DATE.Date    = settingsPacket.data[6];
						device.DATE.Month   = settingsPacket.data[7];
						device.DATE.Year    = settingsPacket.data[8];
						/* обновим значения устройства */
						BaseSettings::getInstance().updateDeviceSettings(device);

	//					updateUi();
					}
					break;
				case RESET_DOOR_EVENT:
					memset(&device,0,sizeof(device));
					device.UPDATE = true;
					device.ENABLE = true;
					device.STATE = BaseSettings::READY;

					/* обновим значения устройства */
					BaseSettings::getInstance().updateDeviceSettings(device);
					break;

				case START_DOWNLOADER:
					qDebug() << "START_DOWNLOADER" ;
					if(settingsPacket.data[0]==1)// все ок
					{
						device.STATE = BaseSettings::FIRMWARE;
						/* обновим значения устройства */
						BaseSettings::getInstance().updateDeviceSettings(device);

						sendData=buff;
						dataCounter=0;

						ui->progressBar->setValue(0);

						sendCommand(WRITE_IMAGE_BLOCK,sendData);

					}
					else
						 QMessageBox::warning(0,"Ошибка","Невозможно перевести устройство в режим загрузки прошивки");
					break;
				case WRITE_IMAGE_BLOCK:
//					qDebug() << "WRITE_IMAGE_BLOCK" ;
					if(settingsPacket.data[0]==1)// все ок
					{
						sendData+=8;
						dataCounter+=8;
						if(dataCounter<bufLenght)
						{
							ui->progressBar->setValue((dataCounter*100)/bufLenght);
							sendCommand(WRITE_IMAGE_BLOCK,sendData);

						}
						else
						{
							qDebug()<<"Write complete";
							ui->progressBar->setValue(0);
					//		settingsPacket.startByte=0xAB;
					//		settingsPacket.command=STOP_DOWNLOADER;


//							delete buff;
							serial->flush();
							sendCommand(STOP_DOWNLOADER);
//							serial->write(reinterpret_cast<char*>(&settingsPacket),sizeof(SettingsPacket));
						}
					}
					break;
				case STOP_DOWNLOADER:
					qDebug() << "STOP_DOWNLOADER" ;
					serial->close();
					device.ENABLE = false;
					device.STATE  = BaseSettings::IDLE;
					/* обновим значения устройства */
					BaseSettings::getInstance().updateDeviceSettings(device);
					if(settingsPacket.data[0] == 0)// ошибка
					{
						QMessageBox::warning(0,"Ошибка", "Ошибка файла!");
		//				device.STATE == BaseSettings::READY;
					}
					else
					{
		//				device.STATE == BaseSettings::READY;
					}
					/* обновим значения устройства */
			//		BaseSettings::getInstance().updateDeviceSettings(device);
					sendCommand(SOFTWARE_RESET);
					break;

			}

		}/* конец обработка команд обмена */
		/* обработка других команд */
		else
		{

		}
	}
}


/*
 * обновим требумые значения если произошло событие
 */
//void MainWindow::updateUi(void)
//{
////	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();

////	if(device.UPDATE)
////	{
//		qDebug() << "updateState";
//		handlerDisplay();
//		handlerEmail();
//		handlerTray();

////		device.UPDATE = false;
////		/* обновим значения устройства */
////		BaseSettings::getInstance().updateDeviceSettings(device);
////	}
//}

//void MainWindow::handleError(QSerialPort::SerialPortError error)
//{
//	 if(error == QSerialPort::NoError) return; //Нет ошибки - нет проблем

//	 if(serial->isOpen())
//		 serial->close();

////	qDebug() << "handleError" ;
//	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();


//	if (error == QSerialPort::ResourceError) {
//		ui->statusBar->showMessage(tr("ResourceError"));
//qDebug() << "ResourceError" ;
//  //      QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
////		closeSerialPort();
//	}
//	if(error == QSerialPort::OpenError)
//	{
//		ui->statusBar->showMessage(tr("OpenError"));
//		qDebug() << "OpenError" ;
//		device.STATE = BaseSettings::SEARCH;
//	}

//	if(error == QSerialPort::TimeoutError)
//	{
//		ui->statusBar->showMessage(tr("TimeoutError"));
//		qDebug() << "TimeoutError" ;
//		device.STATE = BaseSettings::SEARCH;
//	}

//	/* обновим значения устройства */
//	BaseSettings::getInstance().updateDeviceSettings(device);
//	timer_polling->start(1000);
//}

/*
 * вывод значений в форму
 */
void MainWindow::handlerDisplay(void)
{
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();

	//qDebug() << "displayHandler";

	if(!device.ENABLE)
	{
		ui->tab->hide();
		ui->tab_2->show();
		ui->tab_3->hide();
		ui->labelDoorOpen->clear();
		ui->labelDoorOpenTime->clear();
		ui->btnOpenReset->setDisabled(true);
	}
	else
	{
		ui->tab->show();
		ui->tab_2->hide();
		ui->tab_3->hide();
		ui->btnOpenReset->setEnabled(true);
//		this->statusText->setText (QString("Датчик двери"));
//		this->statusText1->setText (QString("Версия %1").arg((char*)(device.VERSION)));

		if(device.DOOR_OPEN)
			ui->labelDoorOpen->setText("Вскрыта");
		else
			ui->labelDoorOpen->setText("Закрыта");
		if(device.COUNT)
		{

			ui->labelDoorOpenTime->setText(QString("%1:%2:%3 %4/%5/%6").arg(device.TIME.Hours).arg(device.TIME.Minutes).arg(device.TIME.Seconds).arg(device.DATE.Date).arg(device.DATE.Month).arg(device.DATE.Year+2000));

//			ui->lineDoorEvent->setText(QString("%1 %2:%3:%4 %5/%6/%7").arg(device.COUNT).arg(device.TIME.Hours).arg(device.TIME.Minutes).arg(device.TIME.Seconds).arg(device.DATE.Date).arg(device.DATE.Month).arg(device.DATE.Year+2000));
			ui->btnOpenReset->setEnabled(true);
		}
		else
		{
			ui->labelDoorOpenTime->clear();
//			ui->lineDoorEvent->clear();
			ui->btnOpenReset->setDisabled(true);
		}
	}
}


/*
 * обработка иконки в трее
 */
void MainWindow::handlerTray(void)
{

	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
	QString str;
//	qDebug() << "trayHandler";
	if(device.ENABLE)
	{
		tray_device->setEnabled(true);
		if(!device.COUNT && !device.DOOR_OPEN)
		{
			trayIcon->setIcon(QIcon(":/Images/door_close.png"));
			trayIcon->showMessage("","");
		}
		else
		{
			trayIcon->setIcon(QIcon(":/Images/door_open.png"));
			trayIcon->showMessage("", QString("%1 %2:%3 %4/%5/%6").arg("Крышка была вскрыта").arg(device.TIME.Hours).arg(device.TIME.Minutes).arg(device.DATE.Date).arg(device.DATE.Month).arg(device.DATE.Year));
		}
	}
	else
	{
		tray_device->setDisabled(true);
		trayIcon->setIcon(QIcon(":/Images/door_no.png"));
		trayIcon->showMessage("", QString("%1").arg("Датчик не обнаружен"));
	}
}
void MainWindow::handlerEmail(void)
{
//	Settings::PROGRAM_SETTINGS_T program = Settings::getInstance().getProgramSettings();
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();

//	if(/*program.alarm_email_enable && */device.COUNT)
	{
	//	static QDateTime datetimeemail = datetime ;

		Settings::EMAIL_SETTINGS_T email = Settings::getInstance ().getEmailSettings();

	////	if( datetime >= datetimeemail)
		{
//			datetimeemail = datetime.addSecs(60 * program.alarm_email_timeout);
			Settings::EMAIL_SETTINGS_T email = Settings::getInstance ().getEmailSettings();
			email.texteditor.clear();
			email.subject.clear();
			email.texteditor.append(QString("%1 %2:%3 %4/%5/%6").arg("Крышка была вскрыта").arg(device.TIME.Hours).arg(device.TIME.Minutes).arg(device.DATE.Date).arg(device.DATE.Month).arg(device.DATE.Year));
			email.subject.append("Аварийное сообщение");

			SendEmail *sendemail = new SendEmail(&email);
			sendemail->sendEmail(&email);
			if(email.enable)
			{
				// в случае отправки таймаут на journal_email_timeout
				trayIcon->showMessage(tr(""), QString("Аварийное сообщение отправлено"));

				BaseSettings::getInstance ().writeFileLog("Аварийное сообщение отправлено");
				qDebug() <<  "email send " ;

			}
			else
			{
				trayIcon->showMessage(tr(""), QString("%1").arg(email.error));
				BaseSettings::getInstance ().writeFileLog(email.error);
				qDebug() <<  "email no send " ;

			}
		}

	}
}

/*
 * Создание трея
 */
void MainWindow::createTray()
{
	tray_settings = new QAction("Настройки", this);
	tray_email    = new QAction("Email", this);
	tray_about    = new QAction("О программе", this);
	tray_device   = new QAction("Прошивка", this);
	tray_update   = new QAction("Обновление", this);
	tray_quit     = new QAction("Выход", this);

	tray_settings->setIcon(QIcon(":/Images/1/Tools.png"));
	tray_email->setIcon(QIcon(":/Images/1/Envelope.png"));
	tray_device->setIcon(QIcon(":/Images/1/Server.png"));
	tray_update->setIcon(QIcon(":/Images/1/Server.png"));
	tray_about->setIcon(QIcon(":/Images/help16.png"));
	tray_quit->setIcon(QIcon(":/Images/application-exit.png"));

	trayIconMenu = new QMenu(this);

	trayIconMenu->addAction(tray_settings);
	trayIconMenu->addAction(tray_email);
	trayIconMenu->addAction(tray_device);
	trayIconMenu->addAction(tray_update);
	trayIconMenu->addAction(tray_about);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(tray_quit);

	tray_update->setDisabled(true);

	trayIcon = new QSystemTrayIcon(this); //инициализируем tray

	trayIcon->setContextMenu(trayIconMenu);

	connect(trayIconMenu, SIGNAL(triggered(QAction*)), SLOT(slotTrayMenu(QAction*)));

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

	trayIcon->setIcon(QIcon(":/Images/door_no.png"));  //устанавливаем иконку

	/* вывод названия при наведении на иконку трея */
	trayIcon->setToolTip(qApp->applicationName());
	trayIcon->show();  //отображаем tray
}

void MainWindow::slotTrayMenu(QAction* pAction)
{
	if(pAction == tray_quit)
	{
		if(serial->isOpen())
			serial->close();
		QApplication::quit();
	}
	else
	{
		timer_polling->stop();
		if(pAction == tray_settings)
		{
			SettingsUpdate *dialog = new SettingsUpdate( 0);
			if (dialog->exec() == QDialog::Accepted)
			{
				dialog->setProgramSettings();
			}
			delete dialog;
		}
		if(pAction == tray_email)
		{
			SettingsUpdate *dialog = new SettingsUpdate( 1);
			if (dialog->exec() == QDialog::Accepted)
			{
				dialog->setEmailSettings();
			}
			delete dialog;
		}
		if(pAction == tray_update)
		{
			frmMain *dialog = new frmMain();
			if (dialog->exec() == QDialog::Accepted)
			{
			}
			delete dialog;
		}

		if(pAction == tray_device)
		{

			ui->tab->hide();
			ui->tab_3->show();

		//	slotStartProg();

//			buff=new unsigned char[bufLenght];

//			UpdateDevice *dialog = new UpdateDevice(0, buff, &bufLenght);
//			if (dialog->exec() == QDialog::Accepted)
//			{
//			}
//			delete dialog;
		}

		if(pAction == tray_about)
		{
			frmAbout *dialog = new frmAbout();
			if (dialog->exec() == QDialog::Accepted)
			{
			}
			delete dialog;
		}
		timer_polling->start();
	}

}


void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	static bool _show = false;
	_show = !_show;
	if(reason == QSystemTrayIcon::Trigger)
	{
		if(_show)
			this->show();
		else
			this->hide();
	}
}


void MainWindow::slotStartProg()
{
	timer_polling->stop();
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();

	if(ui->filePath->text().length()!=0)
	{

		file=new QFile(ui->filePath->text());

		if(file->open(QFile::ReadOnly))
		{
			QByteArray byteArray=file->readAll();

			bufLenght = byteArray.length();
			buff = new unsigned char[bufLenght];

			//копируем образ
			for(int i=0; i<byteArray.length();i++)
				buff[i]=byteArray.data()[i];
			file->close();
			ui->progressBar->setValue(0);
			sendCommand(START_DOWNLOADER);

		}
	}
	timer_polling->start();
}
