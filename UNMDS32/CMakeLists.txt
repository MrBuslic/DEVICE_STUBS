
project(UNMDS32)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON)
set(UNMDS32_SOURCES unmds32.cpp)
include_directories($ENV{LOKI_HOME}/include)	

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

set(UNMDS32_SOURCES unmds32.cpp mds32_rpc.h mds32_rpc.cpp)                                            
include_directories($ENV{VISA_HOME}include)

add_library(unmds32_32 SHARED ${UNMDS32_HEADERS} ${UNMDS32_SOURCES} export.def)
target_link_libraries(unmds32_32 socket_rpc)                                                        

set(RPC_MDS32_SOURCES rpc_mds32.h rpc_mds32.cpp mds32_main.cpp mds32_socket_rpc.h mds32_socket_rpc.cpp)
add_executable(rpc_mds32 WIN32 ${RPC_MDS32_SOURCES})
set_target_properties(rpc_mds32 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mds32 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mds32 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mds32
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mds32.h ${PROJECT_SOURCE_DIR} mds32
)