project(OMNIBUSBOX)
cmake_minimum_required(VERSION 2.8)

#enable_language(C)

set(CMAKE_AUTOMOC ON)
include_directories($ENV{LOKI_HOME}/include)	

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
 find_package (Qt5Script)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories($ENV{MONGO_HOME}/include)
include_directories($ENV{MONGO_HOME}/include/libmongoc-1.0 $ENV{MONGO_HOME}/include/libbson-1.0 $ENV{MONGO_HOME}/include/mongocxx/v_noabi $ENV{MONGO_HOME}/include/bsoncxx/v_noabi)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Qt5Script_INCLUDES})
include_directories(${PROJECT_SOURCE_DIR}/../../lib/db_ctrl)
#include_directories(${PROJECT_SOURCE_DIR}/../../lib/mongo_ctrl)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/mongo_ctrl_v3)

add_definitions(-DNOMINMAX)
add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
if(WIN32)
	set(MY_WIN_FLAG WIN32)
	set(CMAKE_CXX_FLAGS "/EHa /Zc:wchar_t")
	set(ADDITIONAL_LIBS ws2_32)
else()
	set(ADDITIONAL_LIBS pthread mongoclient)
endif()


link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS} $ENV{OMNIBUS_HOME}/lib $ENV{MONGO_HOME}/lib)
set(CARD_SOURCES BTICARD/BTICARD.cpp omnibus_rpc.h omnibus_rpc.cpp BTICARD/BufferClass.h BTICARD/BufferClass.cpp BTICARD/DllMain.cpp)

set(1553_SOURCES BTI1553/BTI1553.cpp omnibus_rpc.h omnibus_rpc.cpp BTI1553/BufferClass1553.h BTI1553/BufferClass1553.cpp BTI1553/DllMain.cpp)

include_directories($ENV{OMNIBUS_HOME}/include)

include_directories(${OMNIBUSBOX_SOURCE_DIR}/../../lib/socket_rpc/include ../../lib/common_functions)

add_library(BTICARD SHARED ${CARD_HEADERS} ${CARD_SOURCES})
add_library(BTI1553 SHARED ${1553_HEADERS} ${1553_SOURCES})

target_link_libraries(BTICARD socket_rpc ${ADDITIONAL_LIBS} ${Qt5Script_LIBRARIES} db_ctrl mongo_controller_v3 ${Boost_LIBRARIES})
target_link_libraries(BTI1553 socket_rpc ${ADDITIONAL_LIBS} ${Qt5Script_LIBRARIES} db_ctrl mongo_controller_v3 ${Boost_LIBRARIES})

set(OMNIBUS_SOURCES OMNIBUSBOX.h OMNIBUSBOX.cpp omnibus_socket_rpc.h omnibus_socket_rpc.cpp main.cpp)

add_executable(rpc_omnibus WIN32 ${OMNIBUS_SOURCES} icon.rc)
set_target_properties(rpc_omnibus PROPERTIES COMPILE_FLAGS "/MP /EHa /Zc:wchar_t /wd\"4503\"" )
target_link_libraries(rpc_omnibus ${QT_LIBRARIES} socket_rpc common_functions ${ADDITIONAL_LIBS})
qt5_use_modules(rpc_omnibus Core Widgets Gui Network)

ADD_CUSTOM_COMMAND(TARGET rpc_omnibus
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/OMNIBUSBOX.h ${PROJECT_SOURCE_DIR} omnibus
)
