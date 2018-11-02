###############################################################################
macro(CONFIGQT)


######################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
   
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	
	if(MSVC)
		set (CMAKE_PREFIX_PATH "C:\\Qt\\5.11.1\\msvc2015")
	endif(MSVC)
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
   
ELSE ()  
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
######################################


	set(CMAKE_INCLUDE_CURRENT_DIR ON) 
	set(CMAKE_AUTOMOC ON)
	
	###############################################################################
SET(QT_USE_QTMAIN TRUE)
SET(QT_DONT_USE_QTGUI TRUE)
###############################################################################
	
	FIND_PACKAGE(Qt5Core)
	FIND_PACKAGE(Qt5Gui)
	FIND_PACKAGE(Qt5Widgets)
	FIND_PACKAGE(Qt5SerialPort)
	FIND_PACKAGE(Qt5Network)
	FIND_PACKAGE(Qt5SerialBus)
	



	LINK_LIBRARIES(		
					Qt5::Widgets
					Qt5::Core
					Qt5::Gui
					Qt5::SerialPort
					Qt5::Network
					Qt5::SerialBus)
	
	INCLUDE_DIRECTORIES(
	QtNetwork
					${Qt5Widgets_INCLUDES}
					${Qt5Core_INCLUDES}
					${Qt5Gui_INCLUDES}
					${Qt5SerialBus_INCLUDES}
					${Qt5SerialPort_INCLUDES}
					${Qt5Network_INCLUDES})
  
endmacro()
###############################################################################