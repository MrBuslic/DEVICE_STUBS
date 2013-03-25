project(UNADS128)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNADS128_SOURCES unads128.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unads128 SHARED ${UNADS128_HEADERS} ${UNADS128_SOURCES} export.def)
