#ifndef MODBUSCVT_H
#define MODBUSCVT_H

#include <QObject>
#include <QModbusDataUnit>
#include <QDataStream>

#include "JQChecksum.h"

#define WRITE_COIL    (0x05)
#define WRITE_M_COIL    (0x0F)

#define READ_COIL     (0x01)

#define WRITE_HOLD    (0x06)
#define READ_HOLD     (0x03)


class ModbusCvt
{
public:
    ModbusCvt();
    static QString  ByteArrayToHexString(QByteArray &ba);
    static unsigned char getModbusCmdType(QModbusDataUnit _ModbusData,int _wr);
    static QByteArray getModbusDataUnitArray(QModbusDataUnit _ModbusData,int _server_addr,int _wr);
};

#endif // MODBUSCVT_H
