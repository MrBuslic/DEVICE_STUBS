project(UNFOI)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNFOI_SOURCES unfoi.cpp)
include_directories($ENV{VISA_HOME}include)

add_library(unfoi_32 SHARED ${UNFOI_HEADERS} ${UNFOI_SOURCES} export.def)
