project(UNVVK4)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNVVK4_SOURCES unvvk4.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unvvk4 SHARED ${UNVVK4_HEADERS} ${UNVVK4_SOURCES} export.def)
