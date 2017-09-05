cmake_minimum_required(VERSION 3.8)
project(clipboard_lite)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
  
  

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

if(WIN32)
    set(CLIPBOARD_LITE_PLATFORM_SRC      
		src/windows/Clipboard.cpp
    )
    set(CLIPBOARD_LITE_PLATFORM_INC     
		include/windows 
    )
elseif(APPLE)
    set(CLIPBOARD_LITE_PLATFORM_SRC
		src/ios/Clipboard.mm
	)

    set(CLIPBOARD_LITE_PLATFORM_INC
		include/ios 
    )
else()
	set(CLIPBOARD_LITE_PLATFORM_SRC 
		src/linux/Clipboard.cpp
    )
	find_package(X11 REQUIRED)
	
	set(CLIPBOARD_LITE_PLATFORM_INC
		include/linux 
		${X11_INCLUDE_DIR}
    ) 
endif()
 
include_directories(	
	include
	${CLIPBOARD_LITE_PLATFORM_INC}
)
add_library(${PROJECT_NAME} 
	src/Clipboard_Lite.cpp 
	${CLIPBOARD_LITE_PLATFORM_SRC}
)
install (TARGETS ${PROJECT_NAME}  
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install (FILES 
	include/Clipboard_Lite.h 
	DESTINATION include
)

 
add_subdirectory(Test)
