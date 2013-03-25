project(NRPZ)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(NRPZ_SOURCES rsnrpz.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(rsnrpz_32 SHARED ${NRPZ_SOURCES} export.def)
