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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>
#include <QTimer>
#include <QMenu>

#include <QtSerialPort/QSerialPort>

#include <QSystemTrayIcon>

//QT_USE_NAMESPACE

//QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

//QT_END_NAMESPACE

//class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public:
	struct SettingsPacket
	{
		quint8 startByte;
		quint8 command;
		quint8 data[16];
	}__attribute__((packed));

private slots:
	void slotPollingDevice();
	void slotResetDoorEvent();
	void slotTrayMenu(QAction* pAction);
	void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
  //  void handleError(QSerialPort::SerialPortError error);

private:
	Ui::MainWindow *ui;
	enum Commands
	{
		READ_ID          = 0x01,
		SET_TIME         = 0x02,
		GET_TIME         = 0x03,
		GET_DOOR_EVENT   = 0x04,
		RESET_DOOR_EVENT = 0x05,
	};

	void sendCommand(Commands command, const void* data = NULL);
	void parseResponsePacket();
//	void updateUi(void);
	void createTray();
	void handlerTray(void);
	void handlerDisplay(void);
	void handlerEmail(void);

	QTimer *timer_polling;

	SettingsPacket settingsPacket;
//    Console *console;
	SettingsDialog *settings;
	QSerialPort    *serial;

	/* tray */
	QAction *tray_settings;
	QAction *tray_email;
	QAction *tray_update;
	QAction *tray_about;
	QAction *tray_quit;

	QSystemTrayIcon *trayIcon;
	QMenu           *trayIconMenu;

};

#endif // MAINWINDOW_H
