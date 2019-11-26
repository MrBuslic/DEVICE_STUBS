
project(UNMT8K4L)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON)
set(UNMT8_SOURCES unmt8k4l.cpp)
include_directories($ENV{VISA_HOME}/include)	

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNMT8K4L_SOURCE_DIR}/../../lib/socket_rpc/include)        
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

set(UNMT8_SOURCES unmt8k4l.cpp mt8k4l_rpc.h mt8k4l_rpc.cpp DllMain.cpp)                                            
include_directories($ENV{VISA_HOME}include)

add_library(unmt8k4l_32 SHARED ${UNMT8_HEADERS} ${UNMT8_SOURCES} export.def)
target_link_libraries(unmt8k4l_32 socket_rpc)                                                        

set(RPC_MT8K4L_SOURCES rpc_mt8k4l.h rpc_mt8k4l.cpp mt8k4l_main.cpp mt8k4l_socket_rpc.h mt8k4l_socket_rpc.cpp)
add_executable(rpc_mt8k4l WIN32 ${RPC_MT8K4L_SOURCES} icon.rc)
set_target_properties(rpc_mt8k4l PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mt8k4l ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mt8k4l Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mt8k4l
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mt8k4l.h ${PROJECT_SOURCE_DIR} mt8k4l
)