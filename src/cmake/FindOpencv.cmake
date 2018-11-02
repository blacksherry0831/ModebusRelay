SET(OpencvInclude_Win
		C:/opencv/build/include
		C:/opencv/build/include/opencv2
		C:/opencv/build/include/opencv
	 )
	 
SET(OpencvInclude_linux
		        /usr/local/include
                /usr/local/include/opencv
                /usr/local/include/opencv2
	 )
#####################################################
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		message(STATUS "using  64bits")
	
else( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		message(STATUS "using  32bits")
	
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
###############################################################################


############≈‰÷√OPENCV#########################################################
macro(ConfigOpenCv)
	if(NEED_OPENCV)
		IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
			FIND_PACKAGE( OpenCV REQUIRED )
			INCLUDE_DIRECTORIES(${OpencvInclude_linux})
			link_libraries(${OpenCV_LIBS})
		ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
			FIND_PACKAGE( OpenCV REQUIRED )
			link_libraries(${OpenCV_LIBS})
		ENDIF () 
	endif(NEED_OPENCV) 
endmacro()


###############################################################################