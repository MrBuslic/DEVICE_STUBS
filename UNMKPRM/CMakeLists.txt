project(UNMKPRM)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNMKPRM_SOURCES unmkprm.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unmkprm_32 SHARED ${UNMKPRM_HEADERS} ${UNMKPRM_SOURCES} export.def)
