# ModebusRelay

使用Modbus协议控制4个继电器的通断

Modbus protocol is used to control the on-off of 4 relays

------

## 协议示例

###    1、0x05-打开一号继电器

​	01 05 00 00 FF 00 00 00

| 字节  | 功能       |
| ----- | ---------- |
| 01    | 模块地址   |
| 05    | 功能码     |
| 00 00 | 继电器地址 |
| FF 00 | 数值       |
| 00 00 | CRC        |

###    2、0x05-关闭一号继电器

​	01 05 00 00 00 00 00 00

###    3、0x0F-打开所有继电器

​	01 0F 00 00 00 04 01 0F 00 00	

###    4、0x0F-关闭所有继电器

​	01 0F 00 00 00 04 01 00 00 00

###    5、0x01 查询所有继电器

​	01 01 00 00 00 04 00 00

| 字节  | 功能           |
| ----- | -------------- |
| 01    | 模块地址       |
| 01    | 读取           |
| 00 00 | 继电器起始地址 |
| 00 04 | 读取长度       |
| 00 00 | CRC            |

### 6、 0x06 修改波特率

​	01 06 02 00 06 00 00 00

| 字节  | 功能       |
| ----- | ---------- |
| 01    | 模块地址   |
| 06    | 设置波特率 |
| 02 00 | 操作地址   |
| 00 0X | 波特率选择 |
| 00 00 | CRC        |

```C++
QStringList relay4::getSupportBaudRate()
  {
      QStringList baud_rate_t;

      baud_rate_t.push_back(tr("1200"));//0
      baud_rate_t.push_back(tr("2400"));//1
      baud_rate_t.push_back(tr("4800"));//2
      baud_rate_t.push_back(tr("9600"));//3
      baud_rate_t.push_back(tr("14400"));//4
      baud_rate_t.push_back(tr("19200"));//5
      baud_rate_t.push_back(tr("38400"));//6

      return baud_rate_t;
  }
```

------

# [Modbus学习网站](https://www.hifreud.com/2016/06/02/modbus-02-simulation-environment-setup/)  