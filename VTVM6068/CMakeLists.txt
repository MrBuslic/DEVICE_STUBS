project(VTVM6068)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(SOURCES Vtvm6068.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(Vtvm6068_32 SHARED ${HEADERS} ${SOURCES} export.def)
