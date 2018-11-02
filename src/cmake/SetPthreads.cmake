
macro(CONFIG_LINUX_BASE)


###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 
 ############################################
 
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	link_libraries(Threads::Threads)
 
 ############################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
############################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
############################################
ELSE ()  
############################################
ENDIF ()  

###############################################################################



endmacro()