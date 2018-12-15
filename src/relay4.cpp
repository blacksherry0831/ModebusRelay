﻿#include "relay4.h"

const int relay4::relay_base_address=RELAY_4_BASE_ADDR;
const int relay4::relay_num=4;


relay4::relay4()
{
	mRegisterType = QModbusDataUnit::RegisterType::Coils;
}


relay4::~relay4()
{

}

QModbusDataUnit relay4::GetReadAllModbusData()
{
    return QModbusDataUnit(mRegisterType, relay_base_address,relay_num);
}

QModbusDataUnit relay4::GetWriteAllModbusData(int _data)
{
    QModbusDataUnit modbusUnit(mRegisterType, relay_base_address, relay_num);
	
	
	for (uint i = 0; i <  modbusUnit.valueCount(); i++) {
		modbusUnit.setValue(i,_data);
	}

	return modbusUnit;

}

QModbusDataUnit relay4::GetWriteOneModbusData(int _addr,int _data)
{
	const int ONE_DATA = 1;
    QModbusDataUnit modbusUnit(mRegisterType, relay_base_address+ _addr, ONE_DATA);
			
	modbusUnit.setValue(0, _data);
	
	return modbusUnit;

}

 QModbusDataUnit relay4::GetWriteBaudRateModbusData(int _data)
 {
     const int ONE_DATA = 1;
     const QModbusDataUnit::RegisterType RegisterType_t=QModbusDataUnit::RegisterType::HoldingRegisters;
     const int relay_register_address_t=RELAY_4_BAUD_RATE_WRITE_ADDRR;
     QModbusDataUnit modbusUnit(RegisterType_t, relay_register_address_t, ONE_DATA);

     modbusUnit.setValue(0, _data);
   //  modbusUnit.setValue(1, _data%256);

     return modbusUnit;

 }
 /**
   * @brief relay4::getSupportBaudRate
   * @return
   */
QStringList relay4::getSupportBaudRate()
  {
      QStringList baud_rate_t;

      baud_rate_t.push_back(tr("1200"));
      baud_rate_t.push_back(tr("2400"));
      baud_rate_t.push_back(tr("4800"));
      baud_rate_t.push_back(tr("9600"));
      baud_rate_t.push_back(tr("14400"));
      baud_rate_t.push_back(tr("19200"));
      baud_rate_t.push_back(tr("38400"));

      return baud_rate_t;
  }
