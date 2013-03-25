project(UNMOSC5)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMOSC5_SOURCES unmosc5.c)
include_directories($ENV{VISA_HOME}/include)

add_library(unmosc5_32 SHARED ${UNMOSC5_HEADERS} ${UNMOSC5_SOURCES} export.def)
