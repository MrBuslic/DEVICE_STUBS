project(UNMBASE)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMBASE_SOURCES unmbase.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmbase_32 SHARED ${UNMBASE_HEADERS} ${UNMBASE_SOURCES} export.def)
