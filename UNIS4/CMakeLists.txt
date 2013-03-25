project(UNIS4)
cmake_minimum_required(VERSION 2.8)

enable_language(C)

set(UNIS4_SOURCES unis4.cpp)
include_directories($ENV{VISA_HOME}/include)

add_library(unis4 SHARED ${UNIS4_HEADERS} ${UNIS4_SOURCES} export.def)
