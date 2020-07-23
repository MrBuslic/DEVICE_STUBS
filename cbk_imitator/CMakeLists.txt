project(cbk_imitator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
enable_language(CXX)
	
find_package(Qt5Core)
find_package (Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)


add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})



include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories($ENV{MONGO_HOME}/include)
include_directories($ENV{MONGO_HOME}/include/libmongoc-1.0 $ENV{MONGO_HOME}/include/libbson-1.0 $ENV{MONGO_HOME}/include/mongocxx/v_noabi $ENV{MONGO_HOME}/include/bsoncxx/v_noabi)

add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(-DNOMINMAX)

include_directories("../../vendor/libgit2/include")
include_directories("../../lib/my_git")
include_directories("../../lib/common_functions")
include_directories("../../lib/db_ctrl")
include_directories("../../lib/mongo_ctrl")
include_directories("../../lib/socket_rpc/include")
include_directories("../OMNIBUSBOX")
include_directories("../UNMDS32")
include_directories("../UNMFSK24")
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
  
SET(CBK_HEADERS cbk_imitator.h CrcUnit.h Integer.h interrupts.h mko_imit.h VIP_imit.h)
SET(CBK_SOURCES cbk_imitator.cpp main.cpp CrcUnit.cpp interrupts.cpp mko_imit.cpp VIP_imit.cpp)




link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS} $ENV{MONGO_HOME}/lib)

SET(HELP_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../UNMDS32/mds32_rpc.h ${PROJECT_SOURCE_DIR}/../UNMFSK24/mfsk24_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp ${PROJECT_SOURCE_DIR}/../UNMDS32/mds32_rpc.cpp ${PROJECT_SOURCE_DIR}/../UNMFSK24/mfsk24_rpc.cpp)
SET(RPC_FOI_RESOURCES ${PROJECT_SOURCE_DIR}/../UNFOI/foi_rpc.h ${PROJECT_SOURCE_DIR}/../UNFOI/foi_rpc.cpp)
set_source_files_properties(${RPC_SOURCES} PROPERTIES GENERATED TRUE)

add_executable(cbk_imitator ${MY_WIN_FLAG} ${CBK_HEADERS} ${CBK_SOURCES} ${HELP_SOURCES} ${RPC_FOI_RESOURCES})
target_link_libraries (cbk_imitator ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} git2 my_git common_functions  mongo_controller_v3 db_ctrl socket_rpc unfoi_32) 

if(WIN32)
  qt5_use_modules(cbk_imitator Core Widgets Network Gui)
endif()







