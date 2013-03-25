project(UNMFSK24)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMFSK24_SOURCES unmfsk24.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmfsk24_32 SHARED ${UNMFSK24_HEADERS} ${UNMFSK24_SOURCES} export.def)
