macro(ConfigCUDA)

 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	  #Searching CUDA
	  FIND_PACKAGE(CUDA) 
	  #Include the FindCUDA script
	  INCLUDE(FindCUDA) 
	  set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11" )
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	  #Searching CUDA
	  FIND_PACKAGE(CUDA) 
	  #Include the FindCUDA script
	  INCLUDE(FindCUDA) 
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

endmacro()