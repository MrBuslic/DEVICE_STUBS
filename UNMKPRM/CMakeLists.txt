project(UNMKPRM)
cmake_minimum_required(VERSION 2.8)

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
include_directories(${UNFOI_SOURCE_DIR}/../../lib/frame_converter)
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

set(UNMKPRM_SOURCES unmkprm.cpp mkprm_rpc.h mkprm_rpc.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmkprm_32 SHARED ${UNMKPRM_HEADERS} ${UNMKPRM_SOURCES} export.def)
target_link_libraries(unmkprm_32 socket_rpc)

set(RPC_MKPRM_SOURCES rpc_mkprm.h rpc_mkprm.cpp mkprm_main.cpp mkprm_socket_rpc.h mkprm_socket_rpc.cpp ${UNFOI_SOURCE_DIR}/../buses_imitator/frame_bus_rpc.h ${UNFOI_SOURCE_DIR}/../buses_imitator/frame_bus_rpc.cpp)
add_executable(rpc_mkprm WIN32 ${RPC_MKPRM_SOURCES} icon.rc)
set_target_properties(rpc_mkprm PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mkprm ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mkprm Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mkprm
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mkprm.h ${PROJECT_SOURCE_DIR} mkprm
)
