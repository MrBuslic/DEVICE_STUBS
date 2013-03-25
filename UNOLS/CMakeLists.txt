project(UNOLS)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNFOI_SOURCES unols.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unols_32 SHARED ${UNFOI_HEADERS} ${UNFOI_SOURCES} export.def)
