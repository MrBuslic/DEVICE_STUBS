project(TEST1003)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(TEST1003_SOURCES test1003.c)
include_directories($ENV{VISA_HOME}/include)

add_library(test1003 SHARED ${TEST1003_HEADERS} ${TEST1003_SOURCES} export.def)
