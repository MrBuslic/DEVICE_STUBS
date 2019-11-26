project(mbk04_imitator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
enable_language(CXX)

include_directories($ENV{LOKI_HOME}/include)	
find_package(Qt5Core)
find_package (Qt5Gui)
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
include_directories("${PROJECT_SOURCE_DIR}/../../lib/socket_rpc/include")
include_directories("${PROJECT_SOURCE_DIR}/../../lib/frame_converter")
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
  
SET(BKUPI_HEADERS mbk04Widget.h)
SET(BKUPI_SOURCES main.cpp mbk04Widget.cpp)



link_directories(${QT_LIBRARY_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS})

SET(RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/frame_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/frame_bus_rpc.cpp mbk04_socket_rpc.h mbk04_socket_rpc.cpp)
SET(POWER_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp)
#set_source_files_properties(${RPC_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(mbk04_imitator ${MY_WIN_FLAG} ${BKUPI_HEADERS} ${BKUPI_SOURCES} ${RPC_SOURCES}  ${POWER_RPC_SOURCES} icon.rc)
target_link_libraries (mbk04_imitator ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc frame_converter ) 

if(WIN32)
  qt5_use_modules(mbk04_imitator Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(mbk04_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()



ADD_CUSTOM_COMMAND(TARGET mbk04_imitator
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/mbk04Widget.h ${PROJECT_SOURCE_DIR} mbk04
)



