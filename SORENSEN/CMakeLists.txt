project(SORENSEN)
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


set(AG7972_SOURCES main.cpp SORENSEN.h SORENSEN.cpp)
SET(KP50_RPC_SOURCES ${PROJECT_SOURCE_DIR}/../UNKP50/kp50_rpc.h ${PROJECT_SOURCE_DIR}/../UNKP50/kp50_rpc.cpp)

include_directories($ENV{VISA_HOME}include)

add_executable(sorensen WIN32 ${AG7972_SOURCES} ${KP50_RPC_SOURCES})
set_target_properties(sorensen PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(sorensen ${QT_LIBRARIES} socket_rpc)
qt5_use_modules(sorensen Core Widgets Gui Network)