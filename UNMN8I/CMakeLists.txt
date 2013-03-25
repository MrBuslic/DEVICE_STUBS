project(UNMN8I)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMN8I_SOURCES unmn8i.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmn8i_32 SHARED ${UNMN8I_HEADERS} ${UNMN8I_SOURCES} export.def)
