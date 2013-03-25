project(UNMDS32)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMDS32_SOURCES unmds32.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmds32_32 SHARED ${UNMDS32_HEADERS} ${UNMDS32_SOURCES} export.def)
