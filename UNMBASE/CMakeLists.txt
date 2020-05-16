project(UNMBASE)
cmake_minimum_required(VERSION 2.8)

enable_language(C)


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

set(UNMBASE_SOURCES unmbase.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmbase_32 SHARED ${UNMBASE_HEADERS} ${UNMBASE_SOURCES} export.def)
target_link_libraries(unmbase_32 ${QT_LIBRARIES} visa32)

qt5_use_modules(unmbase_32 Core)