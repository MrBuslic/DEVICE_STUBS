project(UNRFSW)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNRFSW_SOURCES unrfsw.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unrfsw_32 SHARED ${UNRFSW_HEADERS} ${UNRFSW_SOURCES} export.def)
