CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)

INCLUDE(cmake/FindQT.cmake)
######################################
CONFIGQT()

######################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

qt5_wrap_ui(ui_FILES 
				settingsdialog.ui
				mainwindow.ui)

qt5_add_resources(qrc_FILES 
master.qrc)
######################################
 
   
# Tell CMake to create the helloworld executable
add_executable(modbus_QT  WIN32
                            main.cpp
							relay4.cpp
							SwitchButton.cpp
							settingsdialog.cpp
							mainwindow.cpp
							writeregistermodel.cpp
							${qrc_FILES}
                            ${ui_FILES})



TARGET_LINK_LIBRARIES(modbus_QT 
							) 


######################################
