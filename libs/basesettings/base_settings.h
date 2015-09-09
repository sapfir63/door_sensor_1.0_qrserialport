#ifndef BASESETTINGS_H
#define BASESETTINGS_H
//http://qt.e-werest.org/blog/tutorial/2892.html

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>

//#define DEBUG ;


//template <typename T> class SingleTon {
//public:
//	static T * instance() {
//		static T instance;
//		return &instance;
//	}
// //	static T&    getInstance()
// //	{
// //		static T   s;
// //		return s;
// //	}

//protected:
//	SingleTon()          { }
//	virtual ~SingleTon() { }

// //	SingleTon();
// //	SingleTon(const SingleTon&);
// //	SingleTon& operator= (const SingleTon&);

//};


class BaseSettings //:public SingleTon<BaseSettings>
{
	//Q_OBJECT
public:

//	static BaseSettings * instance()
//	{
//		static BaseSettings instance;
//		return &instance;
//	}

	static BaseSettings&    getInstance()
	{
		static BaseSettings   s;
		return s;
	}

	void createFilesIniLog();

	QString getNameFileIni() const;
	QString getNameFileLog() const;

	void writeFileLog(QString str);

	struct BASE_SERIAL_PORT_S
	{
		bool enable;
		QString port_name;
		quint32 baud_rate;
		quint16 word_length;
		quint16 stop_bits;
		quint16 parity;
		quint16 flow_control;
	} serial;

	typedef struct
	{
	  qint8 Hours;
	  qint8 Minutes;
	  qint8 Seconds;
	}RTC_TimeTypeDef;

	typedef struct
	{
	  qint8 WeekDay;
	  qint8 Month;
	  qint8 Date;
	  qint8 Year;
	}RTC_DateTypeDef;

	enum State
	{
		IDLE = 0,    // начало работы
		SEARCH,      // поиск устройства
		READY        // работа
	};
	struct BASE_DEVICE_SETTINGS_S
	{
		State   STATE;	        // текущее состояние
		bool    ENABLE;	        // флаг нахождения modbus устройства
		bool    NEW;	            // флаг первого соединения
		bool    UPDATE;			// флаг обновления значений
		quint32 COUNT;			// кол-во вскрытий корпуса
		quint8  DOOR_OPEN;		// текущий статус датчика(замкнут/разомкнут)
		quint32 device_id;
	//	qint8  x3_count;	// кол-во  input яйчеек
	//	qint8  x4_count;	// кол-во holding яйчеек
		RTC_DateTypeDef DATE;	//дата последнего срабатывания датчика двери
		RTC_TimeTypeDef TIME;	//время последнего срабатывания датчика двери
		char   VERSION[5];
  //      bool   ext_clock;	// флаг работы от внешнего часового кварца (true внешний false внутренний)
    } device;

//	BASE_SERIAL_PORT_S     serial;
	//BASE_DEVICE_SETTINGS_S device;

	BASE_SERIAL_PORT_S getSerialPortSettings();
	void updateSerialPortSettings(BASE_SERIAL_PORT_S serial);

	BASE_DEVICE_SETTINGS_S getDeviceSettings();
	void updateDeviceSettings(BASE_DEVICE_SETTINGS_S device);

protected:
//	BaseSettings();// {}
//	virtual ~BaseSettings() { }

	BaseSettings();
	BaseSettings(const BaseSettings&);
	BaseSettings& operator= (const BaseSettings&);

//	friend class SingleTon<BaseSettings>;

	void initSerialPortSettings() ;
	void initDeviceSettings() ;

//	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	// файл настроек программы
	QString file_ini;
	// файл сообщений программы
	QString file_log;

};

#endif // BASESETTINGS_H
