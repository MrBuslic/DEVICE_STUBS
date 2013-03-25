project(UNMN3I)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMN3I_SOURCES unmn3i.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmn3i_32 SHARED ${UNMN3I_HEADERS} ${UNMN3I_SOURCES} export.def)