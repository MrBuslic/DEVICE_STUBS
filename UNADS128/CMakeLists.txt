project(UNADS128)
cmake_minimum_required(VERSION 2.8) 
  
#enable_language(C)  
set(CMAKE_AUTOMOC ON) 
set(UNADS128_SOURCES unads128.cpp) 
include_directories($ENV{VISA_HOME}/include) 

find_package(Qt5Core) 
find_package(Qt5Gui)  
find_package(Qt5Widgets) 
find_package(Qt5Network) 
 
include_directories(${Qt5Core_INCLUDES}) 
include_directories(${Qt5Gui_INCLUDES}) 
include_directories(${Qt5Widgets_INCLUDES}) 
include_directories(${Qt5Network_INCLUDES})
include_directories(${UNADS128_SOURCE_DIR}/../../lib/socket_rpc/include)

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

set(UNADS128_SOURCES unads128.cpp unads128_h.h ads128_rpc.h ads128_rpc.cpp)

include_directories($ENV{VISA_HOME}include)

add_library(unads128_32 SHARED ${UNADS128_HEADERS} ${UNADS128_SOURCES} export.def)

target_link_libraries(unads128_32 socket_rpc)

set(RPC_ADS128_SOURCES rpc_ads128.h rpc_ads128.cpp ads128_main.cpp ads128_socket_rpc.h ads128_socket_rpc.cpp)

add_executable(rpc_ads128 WIN32 ${RPC_ADS128_SOURCES})
set_target_properties(rpc_ads128 PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_ads128 ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_ads128 Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_ads128 
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_ads128.h ${PROJECT_SOURCE_DIR} ads128 
) 