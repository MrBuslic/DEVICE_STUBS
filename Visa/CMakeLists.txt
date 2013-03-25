project(VISA)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(SOURCES Visa.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(visa32 SHARED ${HEADERS} ${SOURCES} export.def)
