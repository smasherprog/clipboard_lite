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
if(BUILD_SHARED_LIBS)	
	set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL CLIPBOARD_LITE_DLL)
	if(APPLE)
		find_package(Threads REQUIRED)
		find_library(corefoundation_lib CoreFoundation)
		find_library(cocoa_lib Cocoa)
		target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${corefoundation_lib} ${cocoa_lib})
	else()
		find_package(X11 REQUIRED)
		if(!X11_XTest_FOUND)
 			message(FATAL_ERROR "X11 extensions are required, but not found!")
		endif()
		if(!X11_Xfixes_LIB)
 			message(FATAL_ERROR "X11 fixes extension is required, but not found!")
		endif()
		find_package(Threads REQUIRED)
		target_link_libraries(${PROJECT_NAME} 			
			${X11_LIBRARIES}
			${X11_Xfixes_LIB}
			${X11_XTest_LIB}
			${CMAKE_THREAD_LIBS_INIT}
		)
	endif()
endif()

install (TARGETS ${PROJECT_NAME}  
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install (FILES 
	include/Clipboard_Lite.h 
	DESTINATION include
)

 
add_subdirectory(Test)
