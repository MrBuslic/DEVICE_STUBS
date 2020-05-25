project(PCIMKO)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
enable_language(CXX)

include_directories("../../lib/socket_rpc/include")

SET(RPC_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp)

set(PCIMKO_SOURCES pcimko_imit.cpp)
add_library(PCI_MKO_BASE_DLL SHARED ${PCIMKO_SOURCES} ${RPC_SOURCES} export.def)
target_link_libraries (PCI_MKO_BASE_DLL ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES} socket_rpc)