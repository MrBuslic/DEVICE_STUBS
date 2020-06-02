project(BKIS_imitator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
enable_language(CXX)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories("../../lib/socket_rpc/include")
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

if(WIN32)
	set(MY_WIN_FLAG WIN32)
	set(CMAKE_CXX_FLAGS "/EHa /Zc:wchar_t")
	set(ADDITIONAL_LIBS ws2_32)
else()
	set(ADDITIONAL_LIBS pthread)
endif()

SET(BKIS_HEADERS BKIS.h)
SET(BKIS_SOURCES main.cpp BKIS.cpp)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS})

SET(RPC_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp bkis_socket_rpc.h bkis_socket_rpc.cpp)
SET(MKU_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.cpp)
#SET(INTERRUPT_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.cpp)
SET(POWER_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp)
#set_source_files_properties(${RPC_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(BKIS_imitator ${MY_WIN_FLAG} ${BKIS_HEADERS} ${BKIS_SOURCES} ${RPC_SOURCES} ${MKU_RPC_SOURCES} ${INTERRUPT_RPC_SOURCES} ${POWER_RPC_SOURCES} icon.rc)
target_link_libraries (BKIS_imitator ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

if(WIN32)
  qt5_use_modules(BKIS_imitator Core Widgets Network Gui)
endif()

ADD_CUSTOM_COMMAND(TARGET BKIS_imitator
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/BKIS.h ${PROJECT_SOURCE_DIR} bkis
)

