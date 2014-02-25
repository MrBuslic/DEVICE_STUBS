project(OMNIBUSBOX)
cmake_minimum_required(VERSION 2.8)

enable_language(C)


set(CARD_SOURCES BTICARD.cpp)
set(1553_SOURCES BTI1553.cpp)

include_directories($ENV{OMNIBUS_HOME}/include)

add_library(BTICARD${TEST_LIB} SHARED ${CARD_HEADERS} ${CARD_SOURCES})
add_library(BTI1553${TEST_LIB} SHARED ${1553_HEADERS} ${1553_SOURCES})