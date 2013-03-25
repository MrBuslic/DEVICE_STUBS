project(UNMT8K4L)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMT8_SOURCES unmt8k4l.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmt8k4l_32 SHARED ${UNMT8_HEADERS} ${UNMT8_SOURCES} export.def)
