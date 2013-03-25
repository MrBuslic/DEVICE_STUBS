project(UNMBKOI)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMBKOI_SOURCES unmbkoi.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmbkoi_32 SHARED ${UNMBKOI_HEADERS} ${UNMBKOI_SOURCES} export.def)
