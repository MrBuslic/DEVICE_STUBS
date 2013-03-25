project(UNMS)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMS_SOURCES unms.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unms_32 SHARED ${UNMS_HEADERS} ${UNMS_SOURCES} export.def)
