project(cbk_imitator_tsrp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
enable_language(CXX)
add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

find_package(Qt5Core)
find_package (Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)


add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

add_definitions(-DLOKI_CLASS_LEVEL_THREADING -DWINDOWS -D_WINDOWS)
add_definitions(-DWIN32_LEAN_AND_MEAN)

#include_directories ($ENV{SM_HOME}/target/BOS/BOS_h)
#include_directories ($ENV{SM_HOME}/target/BRPO/BRPO_h)
#include_directories ($ENV{SM_HOME}/target/OPO/OPO_h)
#include_directories ($ENV{SM_HOME}/target/target_h)


include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories($ENV{LOKI_HOME}/include)

add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(-DNOMINMAX)

include_directories("../../vendor/libgit2/include")
include_directories("../../lib/my_git")
include_directories("../../lib/common_functions")
include_directories("../../lib/db_ctrl")
include_directories("../../lib/mongo_ctrl")
include_directories("../../lib/socket_rpc/include")
include_directories("../OMNIBUSBOX")
include_directories("../buses_imitator")

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
	
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
	
if(WIN32)
	set(MY_WIN_FLAG WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa /Zc:wchar_t")
	set(ADDITIONAL_LIBS ws2_32)
else()
	set(ADDITIONAL_LIBS pthread)
endif()
  
FILE(GLOB CBK_HEADERS *.h)
FILE(GLOB CBK_SOURCES *.cpp)

SET(OMNIBUSBOX_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp)
SET(INTERRUPT_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.cpp)
SET(VIP_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp)
SET(MKU_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.cpp)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS} $ENV{MONGO_HOME}/lib)

set_source_files_properties(${RPC_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(cbk_imitator_tsrp ${MY_WIN_FLAG} ${CBK_HEADERS} ${CBK_SOURCES} ${OMNIBUSBOX_SOURCES} ${INTERRUPT_SOURCES} ${VIP_SOURCES} ${MKU_SOURCES} icon.rc)
target_link_libraries (cbk_imitator_tsrp ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

if(WIN32)
  qt5_use_modules(cbk_imitator_tsrp Core Widgets Network Gui)
endif()


#ADD_CUSTOM_COMMAND(TARGET cbk_imitator_tsrp
#	PRE_BUILD
#	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/cbk_imitator.h ${PROJECT_SOURCE_DIR} cbk
#)




