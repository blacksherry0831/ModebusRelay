# ModebusRelay
使用Modbus协议控制4个继电器的通断
# 协议示例
---
   1、0x05-打开一号继电器
   01 05 00 00 FF 00 00 00
+ 01 模块地址
+ 05 功能码
+ 00 00 继电器地址
+ FF 00 数值
+ 00 00 CRC
---
   2、0x05-关闭一号继电器
   01 05 00 00 00 00 00 00
---
   3、0x0F-打开所有继电器
   01 0F 00 00 00 04 01 0F 00 00	
---
   4、0x0F-关闭所有继电器
   01 0F 00 00 00 04 01 00 00 00
---
   5、0x01 查询所有继电器
   01 01 00 00 00 04 00 00
+ 01 模块地址
+ 01 读取
+ 00 00 继电器起始地址
+ 00 04 读取长度
+ 00 00 CRC
   
---
