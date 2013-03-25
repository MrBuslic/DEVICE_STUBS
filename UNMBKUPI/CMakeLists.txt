project(UNMBKUPI)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMBKUPI_SOURCES unmbkupi.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmbkupi_32 SHARED ${UNMBKUPI_HEADERS} ${UNMBKUPI_SOURCES} export.def)
