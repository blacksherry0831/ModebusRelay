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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "writeregistermodel.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>

enum ModbusConnection {
    Serial,
    Tcp
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lastRequest(nullptr)
    , modbusDevice(nullptr)
{
    ui->setupUi(this);

    m_settingsDialog = new SettingsDialog(this);

    initActions();
	initButtonRelay();
	initRelayControls();
	initRelayControlsEvent();

    writeModel = new WriteRegisterModel(this);
    writeModel->setStartAddress(ui->writeAddress->value());
    writeModel->setNumberOfValues(ui->writeSize->currentText());

    ui->writeValueTable->setModel(writeModel);
    ui->writeValueTable->hideColumn(2);
    connect(writeModel, &WriteRegisterModel::updateViewport, ui->writeValueTable->viewport(),
        static_cast<void (QWidget::*)()>(&QWidget::update));

    ui->writeTable->addItem(tr("Coils"), QModbusDataUnit::Coils);
    ui->writeTable->addItem(tr("Discrete Inputs"), QModbusDataUnit::DiscreteInputs);
    ui->writeTable->addItem(tr("Input Registers"), QModbusDataUnit::InputRegisters);
    ui->writeTable->addItem(tr("Holding Registers"), QModbusDataUnit::HoldingRegisters);

    ui->connectType->setCurrentIndex(0);
    on_connectType_currentIndexChanged(0);

    auto model = new QStandardItemModel(10, 1, this);
    for (int i = 0; i < 10; ++i)
        model->setItem(i, new QStandardItem(QStringLiteral("%1").arg(i + 1)));
    ui->writeSize->setModel(model);
    ui->writeSize->setCurrentText("10");
    connect(ui->writeSize,&QComboBox::currentTextChanged, writeModel,
        &WriteRegisterModel::setNumberOfValues);

    auto valueChanged = static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged);
    connect(ui->writeAddress, valueChanged, writeModel, &WriteRegisterModel::setStartAddress);
    connect(ui->writeAddress, valueChanged, this, [this, model](int i) {
        int lastPossibleIndex = 0;
        const int currentIndex = ui->writeSize->currentIndex();
        for (int ii = 0; ii < 10; ++ii) {
            if (ii < (10 - i)) {
                lastPossibleIndex = ii;
                model->item(ii)->setEnabled(true);
            } else {
                model->item(ii)->setEnabled(false);
            }
        }
        if (currentIndex > lastPossibleIndex)
            ui->writeSize->setCurrentIndex(lastPossibleIndex);
    });

	

	this->ModBusDebugMode(false);
	this->InitSerialPortMode(true);

	int width = this->width();;
	int height = this->height();
}

MainWindow::~MainWindow()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;

    delete ui;
}

void MainWindow::initActions()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionOptions->setEnabled(true);

    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::on_connectButton_clicked);
    connect(ui->actionDisconnect, &QAction::triggered,
            this, &MainWindow::on_connectButton_clicked);

    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, m_settingsDialog, &QDialog::show);
}

void MainWindow::on_connectType_currentIndexChanged(int index)
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection> (index);
    if (type == Serial) {
        modbusDevice = new QModbusRtuSerialMaster(this);
    } else if (type == Tcp) {
        modbusDevice = new QModbusTcpClient(this);
        if (ui->portEdit->text().isEmpty())
            ui->portEdit->setText(QLatin1Literal("127.0.0.1:502"));
    }

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {
        ui->connectButton->setDisabled(true);
        if (type == Serial)
            statusBar()->showMessage(tr("Could not create Modbus master."), 5000);
        else
            statusBar()->showMessage(tr("Could not create Modbus client."), 5000);
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onStateChanged);
    }
}

void MainWindow::on_connectButton_clicked()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (static_cast<ModbusConnection> (ui->connectType->currentIndex()) == Serial) {
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                GetSerialPortName());
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                m_settingsDialog->settings().parity);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                m_settingsDialog->settings().baud);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                m_settingsDialog->settings().dataBits);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                m_settingsDialog->settings().stopBits);
        } else {
            const QUrl url = QUrl::fromUserInput(ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        }
        modbusDevice->setTimeout(m_settingsDialog->settings().responseTime);
        modbusDevice->setNumberOfRetries(m_settingsDialog->settings().numberOfRetries);
        if (!modbusDevice->connectDevice()) {
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        } else {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);

			this->on_relay_read_all();

        }
    } else {
        modbusDevice->disconnectDevice();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::onStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState)
        ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));
}

void MainWindow::on_readButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

    if (auto *reply = modbusDevice->sendReadRequest(readRequest(), ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
      
		this->process_resopnse_modbus(reply); 

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
}

void MainWindow::on_writeButton_clicked()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    for (uint i = 0; i < writeUnit.valueCount(); i++) {
        if (table == QModbusDataUnit::Coils)
            writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
        else
            writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
    }

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, ui->serverEdit->value())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                        5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::on_readWriteButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    for (uint i = 0; i < writeUnit.valueCount(); i++) {
        if (table == QModbusDataUnit::Coils)
            writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
        else
            writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
    }

    if (auto *reply = modbusDevice->sendReadWriteRequest(readRequest(), writeUnit,
        ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::on_writeTable_currentIndexChanged(int index)
{
    const bool coilsOrHolding = index == 0 || index == 3;
    if (coilsOrHolding) {
        ui->writeValueTable->setColumnHidden(1, index != 0);
        ui->writeValueTable->setColumnHidden(2, index != 3);
        ui->writeValueTable->resizeColumnToContents(0);
    }

    ui->readWriteButton->setEnabled(index == 3);
    ui->writeButton->setEnabled(coilsOrHolding);
    ui->writeGroupBox->setEnabled(coilsOrHolding);
}

void MainWindow::on_relay_read_all()
{
	relay4 relay4_t;
	relay4_t.relay_addr = ui->serverEdit->value();
	QModbusDataUnit   relay_all = relay4_t.GetReadAllModbusData();

	request_read_modbus_cient(relay_all, relay4_t.relay_addr);

}

void MainWindow::on_relay_open_all()
{
	relay4 relay4_t;
	relay4_t.relay_addr = ui->serverEdit->value();
	QModbusDataUnit   relay_all = relay4_t.GetWriteAllModbusData(1);

	request_write_modbus_cient(relay_all, relay4_t.relay_addr);
}

void MainWindow::on_relay_close_all()
{
	relay4 relay4_t;
	relay4_t.relay_addr = ui->serverEdit->value();
	QModbusDataUnit   relay_all = relay4_t.GetWriteAllModbusData(0);

	request_write_modbus_cient(relay_all, relay4_t.relay_addr);
}

void MainWindow::request_read_modbus_cient(QModbusDataUnit _ModbusData, int _server_addr)
{
	if (!modbusDevice)
		return;
	ui->readValue->clear();
	statusBar()->clearMessage();
	
	if (auto *reply = modbusDevice->sendReadRequest(_ModbusData,_server_addr)) {
		if (!reply->isFinished())
			connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
		else
			delete reply; // broadcast replies return immediately
	}
	else {
		statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
	}

}

void MainWindow::request_write_modbus_cient(QModbusDataUnit _ModbusData, int _server_addr)
{
	if (!modbusDevice)
		return;
	statusBar()->clearMessage();

	QModbusDataUnit writeUnit = _ModbusData;
		
	if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, _server_addr)) {
		if (!reply->isFinished()) {
			connect(reply, &QModbusReply::finished, this, [this, reply]() {
				if (reply->error() == QModbusDevice::ProtocolError) {
					statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
						.arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
						5000);
				}
				else if (reply->error() != QModbusDevice::NoError) {
					statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
						arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
				}

				if (reply->error() == QModbusDevice::NoError) {
					process_resopnse_modbus(reply);
				}

				reply->deleteLater();
			});
		}
		else {
			// broadcast replies return immediately
			reply->deleteLater();
		}
	}
	else {
		statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
	}

}

void MainWindow::process_resopnse_modbus(QModbusReply* reply)
{
	
	if (reply->error() == QModbusDevice::NoError) {
		ui->readValue->clear();
		const QModbusDataUnit unit = reply->result();
		for (uint i = 0; i < unit.valueCount(); i++) {
			const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
				.arg(QString::number(unit.value(i),
					unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
			ui->readValue->addItem(entry);
		}
		processRelayControls(unit);

	}

}

void MainWindow::initRelayControls()
{

	mRelaylabel[0]=ui->label_0_relay;
	mRelaylabel[1]=ui->label_1_relay;
	mRelaylabel[2]=ui->label_2_relay;
	mRelaylabel[3]=ui->label_3_relay;

	mRelayButton[0]=ui->pushButtonRelay0;
	mRelayButton[1]=ui->pushButtonRelay1;
	mRelayButton[2]=ui->pushButtonRelay2;
	mRelayButton[3]=ui->pushButtonRelay3;

	mRelayLabelStatus[0] = ui->label_0_relay_status;
	mRelayLabelStatus[1] = ui->label_1_relay_status;
	mRelayLabelStatus[2] = ui->label_2_relay_status;
	mRelayLabelStatus[3] = ui->label_3_relay_status;

}

void MainWindow::initRelayControlsEvent()
{
	relay4 relay4_t;
	relay4_t.relay_addr = ui->serverEdit->value();

#if 1

	for (size_t bi = 0; bi < relay4_t.relay_num; bi++){

		auto* button = mRelayButton[bi];
#if 1
		connect(button, &QPushButton::clicked, this, [this, button, bi](bool _status) {
			
			int data_t = 0;
			if (button->text().compare(tr("open"))==0) {
				data_t = 1;
			}else {
				data_t = 0;
			}
			relay4 relay4_t;
			relay4_t.relay_addr = ui->serverEdit->value();
			QModbusDataUnit   relay_all = relay4_t.GetWriteOneModbusData(bi, data_t);

			request_write_modbus_cient(relay_all, relay4_t.relay_addr);


	});
#else
connect(button,&SwitchButton::valueChanged, this, [this, button,bi](bool _status) {
		
			int data_t=0;
			if (_status) {
				data_t = 0;
			}else {
				data_t = 1;			
			}

			relay4 relay4_t;
			relay4_t.relay_addr = ui->serverEdit->value();
			QModbusDataUnit   relay_all = relay4_t.GetWriteOneModbusData(bi, data_t);

			request_write_modbus_cient(relay_all, relay4_t.relay_addr);
		
	
		});
#endif // 0
	}

#endif // 0




}

void MainWindow::processRelayControls(QModbusDataUnit _modbusData)
{
	const QString on_color = "color:green;";
	const QString off_color = "color:red;";


	for (uint i = 0; i <_modbusData.valueCount(); i++) {
				QString status_color;
				int relayaddr = i + _modbusData.startAddress();
				int relayValue = _modbusData.value(i);
				if (_modbusData.registerType()==QModbusDataUnit::Coils)	{
								
									if (relayaddr>=0 && relayaddr<4){
													

											if (relayValue) {										
												status_color = on_color;
												mRelayLabelStatus[relayaddr]->setText(tr("on"));
												mRelayButton[relayaddr]->setText(tr("close"));
											}else{												
												status_color = off_color;
												mRelayButton[relayaddr]->setText(tr("open"));
												mRelayLabelStatus[relayaddr]->setText(tr("off"));												
											}
											
											mRelaylabel[relayaddr]->setStyleSheet(status_color);
											mRelayLabelStatus[relayaddr]->setStyleSheet(status_color);

									}


				}
	}
	
}

QModbusDataUnit MainWindow::readRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    int startAddress = ui->readAddress->value();
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->readSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit MainWindow::writeRequest() const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    int startAddress = ui->writeAddress->value();
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(ui->writeSize->currentText().toInt(), 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

void MainWindow::initActionsRelay()
{

}

void MainWindow::initButtonRelay()
{
	connect(ui->pushButton_read_all_relay,SIGNAL(clicked()),this,SLOT(on_relay_read_all()));
	connect(ui->pushButton_open_all_relay, SIGNAL(clicked()), this, SLOT(on_relay_open_all()));
	connect(ui->pushButton_close_all_relay, SIGNAL(clicked()), this, SLOT(on_relay_close_all()));
    connect(ui->pushButton_modifiedBaudRate, SIGNAL(clicked()), this, SLOT(on_pushButton_modifiedBaudRate_clicked()));
}

void MainWindow::ModBusDebugMode(bool _visible)
{
	this->ui->writeGroupBox->setVisible(_visible);
	this->ui->readGroupBox->setVisible(_visible);
	this->ui->writeButton->setEnabled(_visible);
	this->ui->ctrlGroupBox->setVisible(_visible);

}

void MainWindow::InitSerialPortMode(bool _init)
{

	this->ui->comboBox_serial_port->setVisible(_init);

	this->ui->portEdit->setVisible(!_init);

	
	this->InitSerialPortCombox(_init);

	this->setMaximumHeight(270);

}

QString MainWindow::GetSerialPortName()
{
	return ui->comboBox_serial_port->currentText();
}

void MainWindow::InitSerialPortCombox(bool _init)
{
	this->ui->comboBox_serial_port->clear();

	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		qDebug() << "Name : " << info.portName();
		qDebug() << "Description : " << info.description();
		qDebug() << "Manufacturer: " << info.manufacturer();
		qDebug() << "Serial Number: " << info.serialNumber();
		qDebug() << "System Location: " << info.systemLocation();

		this->ui->comboBox_serial_port->addItem(info.portName());
	}

}

void MainWindow::on_pushButton_modifiedBaudRate_clicked()
{
    relay4 relay4_t;
    relay4_t.relay_addr = ui->serverEdit->value();


   const int baudRate_t= this->ui->comboBox_baudRateModified->currentText().toInt();

    QModbusDataUnit   modified_baudrate = relay4_t.GetWriteBaudRateModbusData(baudRate_t);

    request_write_modbus_cient(modified_baudrate, relay4_t.relay_addr);
}
