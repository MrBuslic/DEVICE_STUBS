project(UNMFSK24)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON)
include_directories($ENV{LOKI_HOME}/include)	

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNMFSK24_SOURCE_DIR}/../../lib/socket_rpc/include)        
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

QT5_ADD_RESOURCES(RESOURCES mfsk24.qrc)

set(UNMFSK24_SOURCES unmfsk24.cpp mfsk24_rpc.h mfsk24_rpc.cpp)                                            
include_directories($ENV{VISA_HOME}include)

add_library(unmfsk24_32 SHARED ${UNMFSK24_HEADERS} ${UNMFSK24_SOURCES} export.def)
target_link_libraries(unmfsk24_32 socket_rpc)                                                        

set(RPC_MFSK24_SOURCES rpc_mfsk24.h rpc_mfsk24.cpp mfsk24_main.cpp mfsk24_socket_rpc.h mfsk24_socket_rpc.cpp)
add_executable(rpc_mfsk24 WIN32 ${RPC_MFSK24_SOURCES} ${RESOURCES} icon.rc)
set_target_properties(rpc_mfsk24 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mfsk24 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mfsk24 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mfsk24
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mfsk24.h ${PROJECT_SOURCE_DIR} mfsk24
)