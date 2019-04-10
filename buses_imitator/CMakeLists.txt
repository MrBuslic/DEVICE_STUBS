project(MBK-02_imitator)
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
  

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${Boost_LIBRARY_DIRS})

SET(interrupt_RPC_SOURCES ${PROJECT_SOURCE_DIR}/interrupt_bus_socket_rpc.h ${PROJECT_SOURCE_DIR}/interrupt_bus_socket_rpc.cpp)

add_executable(interrupt_bus ${MY_WIN_FLAG} interrupt_bus.h interrupt_bus.cpp interrupt_main.cpp ${interrupt_RPC_SOURCES})
target_link_libraries (interrupt_bus ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

ADD_CUSTOM_COMMAND(TARGET interrupt_bus
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/interrupt_bus.h ${PROJECT_SOURCE_DIR} interrupt_bus
)

if(WIN32)
  qt5_use_modules(interrupt_bus Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(MBK-02_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()



SET(mku_RPC_SOURCES ${PROJECT_SOURCE_DIR}/mku_bus_socket_rpc.h ${PROJECT_SOURCE_DIR}/mku_bus_socket_rpc.cpp)

add_executable(mku_bus ${MY_WIN_FLAG} mku_bus.h mku_bus.cpp mku_main.cpp ${mku_RPC_SOURCES})
target_link_libraries (mku_bus ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

ADD_CUSTOM_COMMAND(TARGET mku_bus
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/mku_bus.h ${PROJECT_SOURCE_DIR} mku_bus
)

if(WIN32)
  qt5_use_modules(mku_bus Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(MBK-02_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()


SET(kpi_RPC_SOURCES ${PROJECT_SOURCE_DIR}/kpi_bus_socket_rpc.h ${PROJECT_SOURCE_DIR}/kpi_bus_socket_rpc.cpp)

add_executable(kpi_bus ${MY_WIN_FLAG} kpi_bus.h kpi_bus.cpp kpi_main.cpp ${kpi_RPC_SOURCES})
target_link_libraries (kpi_bus ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

ADD_CUSTOM_COMMAND(TARGET kpi_bus
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/kpi_bus.h ${PROJECT_SOURCE_DIR} kpi_bus
)

if(WIN32)
  qt5_use_modules(kpi_bus Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(MBK-02_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()


SET(power_RPC_SOURCES ${PROJECT_SOURCE_DIR}/power_bus_socket_rpc.h ${PROJECT_SOURCE_DIR}/power_bus_socket_rpc.cpp)

add_executable(power_bus ${MY_WIN_FLAG} power_bus.h power_bus.cpp power_main.cpp ${power_RPC_SOURCES})
target_link_libraries (power_bus ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}  ${Qt5Network_LIBRARIES}
                                      ${Boost_LIBRARIES} ${ADDITIONAL_LIBS} socket_rpc) 

ADD_CUSTOM_COMMAND(TARGET power_bus
	PRE_BUILD
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/power_bus.h ${PROJECT_SOURCE_DIR} power_bus
)

if(WIN32)
  qt5_use_modules(power_bus Core Widgets Network Gui)
 # SET_TARGET_PROPERTIES(MBK-02_imitator PROPERTIES LINK_FLAGS  "/NODEFAULTLIB:\"LIBCMTD\"")
endif()
