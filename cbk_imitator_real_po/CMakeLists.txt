project(cbk_imitator_real_po)
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

include_directories ($ENV{SM_HOME}/target/BOS/BOS_h)
include_directories ($ENV{SM_HOME}/target/BRPO/BRPO_h)
include_directories ($ENV{SM_HOME}/target/OPO/OPO_h)
include_directories ($ENV{SM_HOME}/target/target_h)
include_directories (${PROJECT_SOURCE_DIR}/real_tpo/Source)

include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories($ENV{MONGO_HOME}/include)
include_directories($ENV{MONGO_HOME}/include/libmongoc-1.0 $ENV{MONGO_HOME}/include/libbson-1.0 $ENV{MONGO_HOME}/include/mongocxx/v_noabi $ENV{MONGO_HOME}/include/bsoncxx/v_noabi)
include_directories($ENV{LOKI_HOME}/include)

add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(-DNOMINMAX)

FILE(GLOB BOS_SOURCES ${PROJECT_SOURCE_DIR}/real_tpo/MY_BOS/*.cpp ${PROJECT_SOURCE_DIR}/MY_BOS/thread_module.cpp ${PROJECT_SOURCE_DIR}/MY_BOS/interrupt_module.cpp)
FILE(GLOB BOS_HEADERS ${PROJECT_SOURCE_DIR}/real_tpo/MY_BOS/*.h  ${PROJECT_SOURCE_DIR}/MY_BOS/thread_module.h ${PROJECT_SOURCE_DIR}/MY_BOS/interrupt_module.h)

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

SET(TPO_SOURCES ${PROJECT_SOURCE_DIR}/real_tpo/Source/MainUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/DataUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/FuncUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/IrqHandlerUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/MkoXchgUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/ControlUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/CrcUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/CmdHandlerUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/VersionUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/DebugUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/MkoStressTestUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/CmdHandlerErtiUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/IrqHandlerErtiUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/PeriodicMkoMsgUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/ProhibitedMkoMsgUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/PacketMkoMsgUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/MkoMirrorUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/BkoiUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/BkoiTableUnit.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/BaloiUnit_v11.c
	${PROJECT_SOURCE_DIR}/real_tpo/Source/VecUnit.c
)

SET(TPO_HEADERS ${PROJECT_SOURCE_DIR}/real_tpo/Source/MainUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/DataUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/Integer.h

	${PROJECT_SOURCE_DIR}/real_tpo/Source/ControlUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/CrcUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/VersionUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/PeriodicMkoMsgUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/ProhibitedMkoMsgUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/PacketMkoMsgUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/MkoMirrorUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/MkoStressTestUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/BkoiUnit.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/BaloiUnit_v11.h
	${PROJECT_SOURCE_DIR}/real_tpo/Source/VecUnit.h
)
  
SET(CBK_HEADERS cbk_imitator.h)
SET(CBK_SOURCES cbk_imitator.cpp main.cpp)

SET(OMNIBUSBOX_SOURCES ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.h ${PROJECT_SOURCE_DIR}/../OMNIBUSBOX/omnibus_rpc.cpp)
SET(INTERRUPT_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/interrupt_bus_rpc.cpp)
SET(VIP_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/power_bus_rpc.cpp)
SET(MKU_SOURCES ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.h ${PROJECT_SOURCE_DIR}/../buses_imitator/mku_bus_rpc.cpp)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS} $ENV{MONGO_HOME}/lib)

set_source_files_properties(${RPC_SOURCES} PROPERTIES GENERATED TRUE)

add_library(imit_bos ${BOS_SOURCES} ${BOS_HEADERS} ${OMNIBUSBOX_SOURCES} ${INTERRUPT_SOURCES} ${VIP_SOURCES} ${MKU_SOURCES})

add_executable(cbk_imitator_real_po ${MY_WIN_FLAG} ${CBK_HEADERS} ${CBK_SOURCES} ${TPO_SOURCES} ${TPO_HEADERS} icon.rc)
target_link_libraries (cbk_imitator_real_po ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc imit_bos) 

if(WIN32)
  qt5_use_modules(cbk_imitator_real_po Core Widgets Network Gui)
  qt5_use_modules(imit_bos Core Widgets Network Gui)
endif()


ADD_CUSTOM_COMMAND(TARGET cbk_imitator_real_po
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/cbk_imitator.h ${PROJECT_SOURCE_DIR} cbk
)




