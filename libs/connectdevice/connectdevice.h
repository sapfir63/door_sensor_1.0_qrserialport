#ifndef CONNECTDEVICE_H
#define CONNECTDEVICE_H

#include "base_settings.h"
//#include "settings.h"
#include "modbus.h"

class ConnectDevice : public QObject
{
	Q_OBJECT
public:

	ConnectDevice();
//	~ConnectDevice();

	modbus_t  * searchModbusPort(quint16 code, int mb_count);
private:

	bool getDeviceId(quint16 code);

	modbus_t  * modbus;


};

#endif // CONNECTDEVICE_H
