project(UNKP50)
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
include_directories(${UNKP50_SOURCE_DIR}/../../lib/socket_rpc/include)
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

set(UNKP50_SOURCES unkp50.cpp kp50_rpc.h kp50_rpc.cpp DllMain.cpp)
include_directories($ENV{VISA_HOME}include)

add_library(unkp50_32 SHARED ${UNKP50_HEADERS} ${UNKP50_SOURCES} export.def)
target_link_libraries(unkp50_32 socket_rpc)


SET(POWER_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp)
set(RPC_KP50_SOURCES rpc_kp50.h rpc_kp50.cpp kp50_main.cpp kp50_socket_rpc.h kp50_socket_rpc.cpp )
add_executable(rpc_kp50 WIN32 ${RPC_KP50_SOURCES} ${POWER_RPC_SOURCES})
set_target_properties(rpc_kp50 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_kp50 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_kp50 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_kp50
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_kp50.h ${PROJECT_SOURCE_DIR} kp50
)