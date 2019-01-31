project(UNFOI)
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
include_directories(${UNFOI_SOURCE_DIR}/../../lib/socket_rpc/include)
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

set(UNFOI_SOURCES unfoi.cpp foi_rpc.h foi_rpc.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unfoi_32 SHARED ${UNFOI_HEADERS} ${UNFOI_SOURCES} export.def)
target_link_libraries(unfoi_32 socket_rpc)

set(RPC_FOI_SOURCES rpc_foi.h rpc_foi.cpp foi_main.cpp foi_socket_rpc.h foi_socket_rpc.cpp)
add_executable(rpc_foi WIN32 ${RPC_FOI_SOURCES})
set_target_properties(rpc_foi PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_foi ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_foi Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_foi
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_foi.h ${PROJECT_SOURCE_DIR} foi
)