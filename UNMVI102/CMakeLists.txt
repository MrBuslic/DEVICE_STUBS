project(UNMVI102)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMVI102_SOURCES unmvi102.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmvi102_32 SHARED ${UNMVI102_HEADERS} ${UNMVI102_SOURCES} export.def)
