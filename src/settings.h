#ifndef SETTINGS_H
#define SETTINGS_H
//http://qt.e-werest.org/blog/tutorial/2892.html

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>

//#define __DEBUG__ ;

class Settings : public QObject {
	Q_OBJECT
public:
	static Settings&    getInstance()
	{
		static Settings   s;
		return s;
	}

	typedef struct
	{
		bool enable;
		QString error;
		QString host;
		int port;
		bool ssl;
		bool auth;
		QString username;
		QString password;
		QString sender;
		QString recipients;
		QString subject;
		QString texteditor;
		QList<QString> attachments;
	}EMAIL_SETTINGS_T;

	typedef struct
	{
		bool    auto_load;
		bool    alarm_email_enable;		// флаг отправки email
		quint8  alarm_email_timeout;	// время отправки email
		bool    journal_email_enable;	// флаг ведения журнала
		quint8  journal_email_timeout;	// время отправки журнала
		bool    door_state;
    //	quint16 dust_threshold;			// порог пыли
    //	qint8   ow_threshold;			// порог температуры
		bool    tray_enable;			// флаг вывода в трей
		quint8  tray_timeout;			// пауза между выводом сообщения в трее (мин)
	}PROGRAM_SETTINGS_T;

//	typedef struct
//	{
//		bool     active;	// вкл/выкл порта
//		quint16 time_wait;	// время ожидания порта (сек)
//        quint16 time_press; // время удерживания reset  порта  (сек)
//		quint32 time_last;	// время последней перезгрузки (сек)
//	} RESET_SETTINGS_T;


	EMAIL_SETTINGS_T getEmailSettings();
	void updateEmailSettings(EMAIL_SETTINGS_T  email);

	PROGRAM_SETTINGS_T getProgramSettings();
	void updateProgramSettings(PROGRAM_SETTINGS_T program);

//	RESET_SETTINGS_T getResetSettings();
//	void updateResetSettings(RESET_SETTINGS_T reset);

public slots:

signals:

private:

	Settings();
	Settings(const Settings&);
	Settings& operator= (const Settings&);


	void initEmailSettings() ;
	void initProgramSettings();
 //   void initResetSettings();

	EMAIL_SETTINGS_T   email;
	PROGRAM_SETTINGS_T program;
//	RESET_SETTINGS_T   reset;
};

#endif // SETTINGS_H
