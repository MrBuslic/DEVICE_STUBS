project(R732_imitator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
enable_language(CXX)
	
find_package(Qt5Core)
find_package (Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)


add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(-DNOMINMAX)

include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories("../../lib/socket_rpc/include")
include_directories("../../lib/common_functions")
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
include_directories($ENV{MONGO_HOME}/include ${PROJECT_SOURCE_DIR}/../../lib/mongo_ctrl ${PROJECT_SOURCE_DIR}/../../lib/mongo_ctrl_v3 ${PROJECT_SOURCE_DIR}/../../lib/db_ctrl)
include_directories($ENV{MONGO_HOME}/include/libmongoc-1.0 $ENV{MONGO_HOME}/include/libbson-1.0 $ENV{MONGO_HOME}/include/mongocxx/v_noabi $ENV{MONGO_HOME}/include/bsoncxx/v_noabi)
	
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
  
SET(R732_HEADERS R732.h R732Modules.h)
SET(R732_SOURCES main.cpp R732.cpp)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS}  $ENV{MONGO_HOME}/lib)

SET(RPC_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp ${PROJECT_SOURCE_DIR}/../MBK-02_imitator/MBK02_rpc.h 
	${PROJECT_SOURCE_DIR}/../MBK-02_imitator/MBK02_rpc.cpp ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.cpp  
	${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp r732_socket_rpc.h r732_socket_rpc.cpp)

add_executable(R732_imitator ${MY_WIN_FLAG} ${R732_HEADERS} ${R732_SOURCES} ${RPC_SOURCES})
target_link_libraries (R732_imitator ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc common_functions mongo_controller_v3 db_ctrl) 

if(WIN32)
  qt5_use_modules(R732_imitator Core Widgets Network Gui)
endif()


ADD_CUSTOM_COMMAND(TARGET R732_imitator
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/R732.h ${PROJECT_SOURCE_DIR} r732
)


