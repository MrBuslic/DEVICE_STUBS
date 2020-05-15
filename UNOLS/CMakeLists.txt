project(UNOLS)
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
include_directories(${PROJECT_SOURCE_DIR}/../../lib/socket_rpc/include)
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

set(UNOLS_SOURCES unols.cpp unols_h.h ols_rpc.h ols_rpc.cpp DllMain.cpp)
include_directories($ENV{VISA_HOME}include)

add_library(unols_32 SHARED ${UNOLS_HEADERS} ${UNOLS_SOURCES} export.def)
target_link_libraries(unols_32 socket_rpc visa32)

set(RPC_OLS_SOURCES rpc_ols.h rpc_ols.cpp ols_main.cpp ols_socket_rpc.h ols_socket_rpc.cpp)
add_executable(rpc_ols WIN32 ${RPC_OLS_SOURCES})
set_target_properties(rpc_ols PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_ols ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(rpc_ols Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_ols
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/rpc_ols.h ${PROJECT_SOURCE_DIR} ols
)

