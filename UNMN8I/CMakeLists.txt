project(UNMN8I)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)
set(CMAKE_AUTOMOC ON)
set(UNMN8I_SOURCES unmn8i.cpp)
include_directories($ENV{VISA_HOME}/include)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNMN8I_SOURCE_DIR}/../../lib/socket_rpc/include)

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

set(UNMN8I_SOURCES unmn8i.cpp unmn8i_h.h mn8i_rpc.h mn8i_rpc.cpp DllMain.cpp)

include_directories($ENV{VISA_HOME}include)

add_library(unmn8i_32 SHARED ${UNMN8I_HEADERS} ${UNMN8I_SOURCES} export.def)

target_link_libraries(unmn8i_32 socket_rpc)

set(RPC_MN8I_SOURCES rpc_mn8i.h rpc_mn8i.cpp 
	../buses_imitator/mku_bus_rpc.h ../buses_imitator/mku_bus_rpc.cpp
	mn8i_main.cpp mn8i_socket_rpc.h mn8i_socket_rpc.cpp)

add_executable(rpc_mn8i WIN32 ${RPC_MN8I_SOURCES} icon.rc)
set_target_properties(rpc_mn8i PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_mn8i ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_mn8i Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_mn8i
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_mn8i.h ${PROJECT_SOURCE_DIR} mn8i
)

