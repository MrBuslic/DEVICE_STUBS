project(UNRBSTD)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNRBSTD_SOURCES unrbstd.cpp)
add_library(unrbstd_32 SHARED ${UNRBSTD_SOURCES} export.def)
