#include "relay4.h"

const int relay4::relay_address=0;
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
	return QModbusDataUnit(mRegisterType, relay_address,relay_num);
}

QModbusDataUnit relay4::GetWriteAllModbusData(int _data)
{
	QModbusDataUnit modbusUnit(mRegisterType, relay_address, relay_num);
	
	
	for (uint i = 0; i <  modbusUnit.valueCount(); i++) {
		modbusUnit.setValue(i,_data);
	}

	return modbusUnit;

}

QModbusDataUnit relay4::GetWriteOneModbusData(int _addr,int _data)
{
	const int ONE_DATA = 1;
	QModbusDataUnit modbusUnit(mRegisterType, relay_address+ _addr, ONE_DATA);
			
	modbusUnit.setValue(0, _data);
	
	return modbusUnit;

}
