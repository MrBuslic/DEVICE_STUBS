project(SCHWARZ)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(SCHWARZ_SOURCES rssmf.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(rssmf_32 SHARED ${SCHWARZ_SOURCES} export.def)
