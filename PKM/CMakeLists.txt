project(PKM)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
enable_language(CXX)

include_directories("../../lib/socket_rpc/include")
add_definitions(-D_AFXDLL)
SET(RPC_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp)
SET(INTERRUPT_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.cpp)
set(PKM_SOURCES MAIN_OBJ.h PKM.cpp)
add_library(PKM SHARED ${PKM_SOURCES} ${RPC_SOURCES} ${INTERRUPT_RPC_SOURCES} export.def)
target_link_libraries (PKM ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES} socket_rpc)