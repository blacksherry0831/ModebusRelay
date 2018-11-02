
function (ConfigMFC )  


IF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
   SET(CMAKE_MFC_FLAG 2)	
ENDIF()  
  
endfunction () 


macro(ConfigMFC_2)
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	if(MSVC_VERSION)
		 SET(CMAKE_MFC_FLAG 2)	
    endif(MSVC_VERSION)
    
ENDIF() 
endmacro()