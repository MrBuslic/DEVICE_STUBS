project(UNVVK4)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNVVK4_SOURCE_DIR}/../../lib/socket_rpc/include)
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


set(UNVVK4_SOURCES unvvk4.cpp vvk4_rpc.h vvk4_rpc.cpp DllMain.cpp)
include_directories($ENV{LOKI_HOME}/include)	
include_directories($ENV{VISA_HOME}/include)

add_library(unvvk4 SHARED ${UNVVK4_HEADERS} ${UNVVK4_SOURCES} export.def)
target_link_libraries(unvvk4 socket_rpc)

set(RPC_VVK4_SOURCES rpc_vvk4.h rpc_vvk4.cpp vvk4_main.cpp vvk4_socket_rpc.h vvk4_socket_rpc.cpp)
add_executable(rpc_vvk4 WIN32 ${RPC_VVK4_SOURCES} icon.rc)
set_target_properties(rpc_vvk4 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_vvk4 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_vvk4 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_vvk4
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_vvk4.h ${PROJECT_SOURCE_DIR} vvk4
)