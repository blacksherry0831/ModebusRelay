#pragma once

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>

class relay4:public QObject
{
public:
	relay4();
	~relay4();
public:
	static const int relay_address;
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

