QT += serialbus serialport widgets
requires(qtConfig(combobox))

TARGET = modbusmaster
TEMPLATE = app
CONFIG += c++11

TRANSLATIONS += Resource/main_widget_zh.ts \
               Resource/main_widget_en.ts

SOURCES += main.cpp\
        mainwindow.cpp \
        settingsdialog.cpp \
        writeregistermodel.cpp \
    relay4.cpp

HEADERS  += mainwindow.h \
         settingsdialog.h \
        writeregistermodel.h \
    relay4.h

FORMS    += mainwindow.ui \
         settingsdialog.ui

RESOURCES += \
    master.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
INSTALLS += target
