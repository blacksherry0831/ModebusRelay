#include "modbuscvt.h"

ModbusCvt::ModbusCvt()
{

}
QByteArray ModbusCvt::getModbusDataUnitArray(QModbusDataUnit _ModbusData,int _server_addr,int _wr)
{
    int func_code= ModbusCvt::getModbusCmdType(_ModbusData,_wr);

    QByteArray qba;
                qba.append((char)_server_addr)
                    .append(func_code)
                      .append(_ModbusData.startAddress()>>8)
                        .append(_ModbusData.startAddress()&0x00ff);
      if(func_code==READ_COIL){
          //读线圈
        qba.append(_ModbusData.valueCount()>>8);
        qba.append(_ModbusData.valueCount()&0x00ff);
      }else if(func_code == WRITE_HOLD){
        qba.append(_ModbusData.values().at(0)>>8);
        qba.append(_ModbusData.values().at(0) & 0xff);
      }else if(func_code == WRITE_COIL){
          if(_ModbusData.values().at(0)){
              qba.append(0xFF);
         }else{
              qba.append((char)0x00);
         }
          qba.append((char)0x00);
      }else if(func_code == WRITE_M_COIL){
          qba.append(_ModbusData.valueCount()>>8);
          qba.append(_ModbusData.valueCount()&0x00ff);
          qba.append(1);//字节数
           if(_ModbusData.values().at(0)){
               qba.append(0x0F);
          }else{
               qba.append((char)0x00);
          }

      }else{

      }

    int crc_t=JQChecksum::crc16ForModbus(qba);
     qba.append(crc_t&0x00ff);
     qba.append(crc_t>>8);
    return qba;

}

unsigned char  ModbusCvt::getModbusCmdType(QModbusDataUnit _ModbusData,int _wr)
{
    if(_ModbusData.registerType()==QModbusDataUnit::RegisterType::Coils){

            if(_wr){
                if(_ModbusData.valueCount()==1){
                    return 0x05;//写单个线圈
                }else{
                    return 0x0F;//写多个线圈
                }

            }else{
                    return 0x01;//读线圈
           }

    }else if(_ModbusData.registerType()==QModbusDataUnit::RegisterType::HoldingRegisters){
            if(_wr){
                    return 0x06;//写保持
            }else{
                    return 0x03;//读保持
            }
    }else{

    }
    return 0;
}

QString ModbusCvt::ByteArrayToHexString(QByteArray &ba)
{
    QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
    QString buf;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每次一个字节的填充到 outchar
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
        buf += str;
    }
    return buf;
}
