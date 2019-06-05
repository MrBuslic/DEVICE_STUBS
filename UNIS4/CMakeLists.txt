project(UNIS4)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON) 
set(UNIS4_SOURCES unis4.cpp)

include_directories($ENV{VISA_HOME}/include)

find_package(Qt5Core) 
find_package(Qt5Gui)  
find_package(Qt5Widgets) 
find_package(Qt5Network) 
 
include_directories(${Qt5Core_INCLUDES}) 
include_directories(${Qt5Gui_INCLUDES}) 
include_directories(${Qt5Widgets_INCLUDES}) 
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNIS4_SOURCE_DIR}/../../lib/socket_rpc/include)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
if(WIN32)
	set(CMAKE_CXX_FLAGS "/EHa")
endif()

set(UNIS4_SOURCES unis4.cpp is4_rpc.h is4_rpc.cpp)

include_directories($ENV{VISA_HOME}include)

add_library(unis4 SHARED ${UNIS4_HEADERS} ${UNIS4_SOURCES} export.def)

target_link_libraries(unis4 socket_rpc)

set(RPC_IS4_SOURCES rpc_is4.h rpc_is4.cpp is4_main.cpp is4_socket_rpc.h is4_socket_rpc.cpp)

add_executable(rpc_is4 WIN32 ${RPC_IS4_SOURCES})
set_target_properties(rpc_is4 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_is4 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_is4 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_is4 
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_is4.h ${PROJECT_SOURCE_DIR} is4 
) 
