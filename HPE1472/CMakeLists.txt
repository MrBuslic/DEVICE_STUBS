project(HPE1472)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(HPE1472_SOURCES hpe1472.c)
include_directories($ENV{VISA_HOME}/include)
add_library(hpe1472_32 SHARED ${HPE1472_HEADERS} ${HPE1472_SOURCES} export.def)
