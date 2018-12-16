QT += serialbus serialport widgets

#requires(qtConfig(combobox))
#qt-opensource-windows-x86-mingw492-5.6.3.exe


TARGET = modbusmaster
TEMPLATE = app
CONFIG += c++11

TRANSLATIONS += translations/main_widget_zh.ts \
               translations/main_widget_en.ts

SOURCES += main.cpp\
        mainwindow.cpp \
        settingsdialog.cpp \
        writeregistermodel.cpp \
		relay4.cpp \
		JQChecksum.cpp \
    modbuscvt.cpp

HEADERS  += mainwindow.h \
         settingsdialog.h \
        writeregistermodel.h \
		relay4.h \
		JQChecksum.h \
    modbuscvt.h

FORMS    += mainwindow.ui \
         settingsdialog.ui

RESOURCES += \
    master.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
INSTALLS += target

DISTFILES += \
    translations/main_widget_zh.ts
