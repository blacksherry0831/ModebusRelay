/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the QtSerialBus module.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define USE_MODBUS_CLIENT_OR_SERIALPORT (0)

#if 1
#include <QMainWindow>
#include <QModbusDataUnit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include <QMessageBox>
#endif

#if QT_VERSION

#include <SwitchButton.hpp>
#include "relay4.h"
#include "modbuscvt.h"

#endif

QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

namespace Ui {
class MainWindow;
class SettingsDialog;
}

QT_END_NAMESPACE

class SettingsDialog;
class WriteRegisterModel;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initActions();
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
	void initActionsRelay();
	void initButtonRelay();
    void initComboxBoxBaudRate_all();
    void initComboxBoxBaudRate(QComboBox* _baud_rate) const;
	void ModBusDebugMode(bool _visible);
	void InitSerialPortMode(bool _init);
	void InitSerialPortCombox(bool _init);
	QString GetSerialPortName();
    int GetSerialPortBaudrate();

    void SetTcpModbusParam();
    void serial_port_connect_disconnedt();
    void modbus_rtu_connect_disconnedt();

public slots:
    void modbusDeviceDisCconnected();
    void on_connectButton_clicked();
    void onStateChanged(int state);

    void on_readButton_clicked();
    void readReady();

    void on_writeButton_clicked();
    void on_readWriteButton_clicked();

    void on_connectType_currentIndexChanged(int);
    void on_writeTable_currentIndexChanged(int);


    void serialportWrite(QByteArray _qba,QModbusDataUnit _ModbusData);
    void request_read_modbus_cient_serialport(QModbusDataUnit _ModbusData,int _server_addr);
    void request_write_modbus_cient_serialport(QModbusDataUnit _ModbusData, int _server_addr);

    void request_read_modbus_cient_ModbusClient(QModbusDataUnit _ModbusData,int _server_addr);
    void request_write_modbus_cient_ModbusClient(QModbusDataUnit _ModbusData, int _server_addr);


	void request_read_modbus_cient(QModbusDataUnit _ModbusData,int _server_addr);
	void request_write_modbus_cient(QModbusDataUnit _ModbusData, int _server_addr);

	void process_resopnse_modbus(QModbusReply* reply);
    void on_pushButton_modifiedBaudRate_clicked();
    void showSettingsDialog();
public slots:
    void on_relay_read_all();
    void on_relay_open_all();
    void on_relay_close_all();

    void onQModbusClient_errorOccurred(int _state);

    void on_modbusSerialport_ready_read();

private:

	QLabel* mRelaylabel[4];
	QLabel* mRelayLabelStatus[4];
	QPushButton* mRelayButton[4];
	void initRelayControls();
	void initRelayControlsEvent();
	void processRelayControls(QModbusDataUnit _data);
    void showstatusbar_modbus_data(const QModbusDataUnit _unit);
    void statusBar_showMessage(QString _msg,int _timeout);
    int  isModbusConnected();

private:
    Ui::MainWindow *ui;
    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;
#if !USE_MODBUS_RTU_OR_SERIALPORT
        QSerialPort     *modbusSerialport;
        QByteArray       modbusSerialportByte;
        QModbusDataUnit  modbusSerialportData;
#endif
    SettingsDialog *m_settingsDialog;
    WriteRegisterModel *writeModel;
};

#endif // MAINWINDOW_H
