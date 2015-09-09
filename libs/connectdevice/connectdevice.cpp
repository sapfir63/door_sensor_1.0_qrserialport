#include <QRegExp>

#include "connectdevice.h"
#include "qextserialenumerator.h"
#include "modbus_map.h"
#include "base_settings.h"


//extern modbus_t *m_modbus;

ConnectDevice::ConnectDevice(){}

const QString getSerialPortName(QString  & s )
{
//	QString s2 = s.section( '(', 1 ).section( ')', 0, 0 );

	QRegExp r("([aA-zZ]+)([0-9]+)");
	r.indexIn(s);
	int p = r.cap(2).toInt();
	QString s1 = r.cap(1);

	qDebug() <<  "get number" <<  QString("%1 %2").arg( s1).arg(p );
	QString s3 = "";
	if(p > 9)
		s3.append("\\\\.\\").append(s1).append(QString("%1").arg(p).toLatin1());
	else
		s3.append(s1).append(QString("%1").arg(p).toLatin1()).append(":");
	qDebug() <<  "SerialPortName" <<   s3;
	return s3;
}

/*
 * Поиск  Modbus Port
 * code     код нужного устройства порта
 * mb_count  кол-во сканируемых портов
 */
modbus_t * ConnectDevice::searchModbusPort(quint16 code,int mb_count)
{
	BaseSettings::BASE_SERIAL_PORT_S serial  = BaseSettings::getInstance().getSerialPortSettings ();
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings ();

	char parity;
	switch( serial.parity )
	{
		case 2: parity = 'O'; break;
		case 1: parity = 'E'; break;
		default:
		case 0: parity = 'N'; break;
	}

	if(device.modbus_id < 0 && device.modbus_id > 247)
		device.modbus_id = 1;

	// проверка на наличие порта в ini файле
	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();


		for (qint8 i = 0; i < ports.size(); i++)
			qDebug() <<  "friendName  baud ports.size(" <<  QString("%1 %2 %3").arg( ports[i].friendName).arg(ports[i].physName).arg(ports[i].portName);

    /*for (qint8 i = 0; i < ports.size(); i++)
	{
		if(ports[i].portName == serial.port_name)
		{
			qDebug() <<  "presence port в ini file ";

			const QString port = getSerialPortName( serial.port_name);

			modbus = modbus_new_rtu( port.toLatin1().constData(), serial.baud_rate, parity, serial.word_length, serial.stop_bits );
			if( modbus_connect( modbus ) != -1  )
			{
				modbus_set_slave( modbus, device.modbus_id );

				if(this->getDeviceId(code))
				{
					modbus_close(modbus);
					return modbus;
				}
			}
			modbus_close(modbus);
		}
    }*/

	modbus = NULL;

//	int ret = -1;

//	if( modbus )
//		modbus = NULL;

	for (qint8 i = 0; i < ports.size(); i++)
	{
		qDebug() <<  "no presence port в ini file";

		const QString port = getSerialPortName( ports[i].friendName );

		qDebug() <<  "friendName  baud ports.size(" <<  QString("%1 %2 %3").arg( ports[i].friendName).arg(serial.baud_rate).arg(ports.size());


		modbus = modbus_new_rtu( port.toLatin1().constData(), serial.baud_rate, parity, serial.word_length, serial.stop_bits );

		if( modbus_connect( modbus ) == -1 )
			modbus = NULL;
		else
		{
			for(qint8 j = 1; j < mb_count + 1; j++) // поиск адреса modbus устройства
			{
				modbus_set_slave( modbus, j );

				qDebug() <<  "scan modbus address " <<  QString("%1").arg(j);

				if(this->getDeviceId(code))
				{
					serial.port_name = ports[i].portName;

					BaseSettings::getInstance ().updateSerialPortSettings (serial);
					device = BaseSettings::getInstance ().getDeviceSettings();

					device.modbus_id = j;
					device.update = true;
					BaseSettings::getInstance ().updateDeviceSettings(device);

					modbus_close(modbus);

					return modbus;
				}
            }
            modbus_close(modbus);
		}

	}
	return NULL;
}
/*
*  запрос кода устройства и версии программы
*/
bool ConnectDevice::getDeviceId(quint16 code)
{
//	Settings  &  settings = Settings::getInstance();
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings ();

	int ret = -1;
	uint16_t dest16[10];

	ret = modbus_read_input_registers ( modbus, X3_DEVICE_CODE, 6, dest16 );
	if(ret == -1)
		return false;

	device.device_id = dest16[0];

	// получим версию прошивки
	device.version[0]= (uint8_t) dest16[1] & 0xff;
	device.version[1]= (uint8_t) dest16[2] & 0xff;
	device.version[2]= (uint8_t) dest16[3] & 0xff;
	device.x3_count  = (uint8_t) dest16[4] & 0xff;
	device.x4_count  = (uint8_t) dest16[5] & 0xff;

	BaseSettings::getInstance().updateDeviceSettings(device);
	if(device.device_id == code)
		return true;

	return false;
}
