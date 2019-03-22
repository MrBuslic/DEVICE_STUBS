project(KPRD_imitator)
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
  
FILE(GLOB SOURCES *.cpp)
FILE(GLOB HEADERS *.h *.hpp)  
  
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS})

SET(RPC_SOURCES  ${PROJECT_SOURCE_DIR}/../UNOLS/ols_rpc.h ${PROJECT_SOURCE_DIR}/../UNOLS/ols_rpc.cpp)

add_executable(KPRD_imitator ${MY_WIN_FLAG} ${HEADERS} ${SOURCES} ${RPC_SOURCES})
target_link_libraries (KPRD_imitator ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

if(WIN32)
  qt5_use_modules(KPRD_imitator Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(KPRD_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()



#ADD_CUSTOM_COMMAND(TARGET KPRD_imitator
#	PRE_BUILD
#	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/KPRD_imitator.h ${PROJECT_SOURCE_DIR} kprd
#)



