project(UNKP50)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNKP50_SOURCES unkp50.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unkp50_32 SHARED ${UNKP50_HEADERS} ${UNKP50_SOURCES} export.def)
