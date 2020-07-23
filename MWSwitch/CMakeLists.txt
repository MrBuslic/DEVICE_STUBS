
project(MWSwitch)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNMDS32_SOURCE_DIR}/../../lib/socket_rpc/include)        
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

set(MWSWITCH_SOURCES mwswitch.cpp mwswitch_rpc.h mwswitch_rpc.cpp)                                            
include_directories($ENV{IVI_HOME}/include)

add_library(AgMWSwitch SHARED ${MWSWITCH_SOURCES} export.def)
target_link_libraries(AgMWSwitch socket_rpc)                                                        

set(RPC_mwswitch_SOURCES rpc_mwswitch.h rpc_mwswitch.cpp mwswitch_main.cpp mwswitch_socket_rpc.h mwswitch_socket_rpc.cpp)
add_executable(rpc_mwswitch WIN32 ${RPC_mwswitch_SOURCES} icon.rc)
set_target_properties(rpc_mwswitch PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mwswitch ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mwswitch Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mwswitch
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mwswitch.h ${PROJECT_SOURCE_DIR} mwswitch
)