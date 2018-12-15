#pragma once

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>

#define RELAY_4_BASE_ADDR           (0x0)
#define RELAY_4_BAUD_RATE_WRITE_ADDRR     (0x0200)
/**
 * @brief The relay4 class
 */
class relay4:public QObject
{
public:
	relay4();
	~relay4();
public:
    static const int relay_base_address;
	static const int relay_num;
public:
	 int  relay_addr;
private:
	 QModbusDataUnit::RegisterType mRegisterType;
public:
     static QStringList getSupportBaudRate();
public:
	QModbusDataUnit GetReadAllModbusData();
	QModbusDataUnit GetWriteAllModbusData(int _data);

	QModbusDataUnit GetWriteOneModbusData(int _addr, int _data);

    QModbusDataUnit GetWriteBaudRateModbusData(int _data);

};

