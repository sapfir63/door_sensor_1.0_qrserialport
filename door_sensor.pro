QT += core gui widgets serialport network

TARGET = door_sensor
TEMPLATE = app

INCLUDEPATH += src \
		libs/basesettings/ \
		libs/connectdevice/ \
		libs/smtpclient/ \
		libs/device_code/


SOURCES += \
	src/main.cpp \
	src/mainwindow.cpp \
	src/settingsdialog.cpp \
	src/console.cpp \
    libs/basesettings/base_settings.cpp \
    src/frmabout.cpp \
    src/frmmain.cpp \
    src/LWVersion.cpp \
    src/sendemail.cpp \
    src/settings.cpp \
    src/settingsprogram.cpp \
    src/settingsupdate.cpp \
    libs/smtpclient/emailaddress.cpp \
    libs/smtpclient/mimeattachment.cpp \
    libs/smtpclient/mimecontentformatter.cpp \
    libs/smtpclient/mimefile.cpp \
    libs/smtpclient/mimehtml.cpp \
    libs/smtpclient/mimeinlinefile.cpp \
    libs/smtpclient/mimemessage.cpp \
    libs/smtpclient/mimemultipart.cpp \
    libs/smtpclient/mimepart.cpp \
    libs/smtpclient/mimetext.cpp \
    libs/smtpclient/quotedprintable.cpp \
    libs/smtpclient/smtpclient.cpp \
    src/msgutilsmy.cpp \
#	src/updatedevice.cpp

HEADERS += \
	src/mainwindow.h \
	src/settingsdialog.h \
	src/console.h \
    libs/basesettings/base_settings.h \
    libs/device_code/device_code.h \
    src/frmabout.h \
    src/version.h \
    src/frmmain.h \
    src/LWVersion.h \
    src/sendemail.h \
    src/settings.h \
    src/settingsprogram.h \
    src/settingsupdate.h \
    libs/smtpclient/emailaddress.h \
    libs/smtpclient/mimeattachment.h \
    libs/smtpclient/mimecontentformatter.h \
    libs/smtpclient/mimefile.h \
    libs/smtpclient/mimehtml.h \
    libs/smtpclient/mimeinlinefile.h \
    libs/smtpclient/mimemessage.h \
    libs/smtpclient/mimemultipart.h \
    libs/smtpclient/mimepart.h \
    libs/smtpclient/mimetext.h \
    libs/smtpclient/quotedprintable.h \
    libs/smtpclient/smtpclient.h \
    libs/smtpclient/SmtpMime \
    src/msgutilsmy.h \
#	src/updatedevice.h

FORMS += \
	forms/mainwindow.ui \
	forms/settingsdialog.ui \
    forms/frmabout.ui \
    forms/settingsemail.ui \
    forms/settingsprogram.ui \
    forms/frmmain.ui \
#	forms/updatedevice.ui

RESOURCES += \
    qrc/door_sensor.qrc
