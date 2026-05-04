cmake_minimum_required(VERSION 3.16)
project(LOVEPower VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -lm")

find_package(PkgConfig REQUIRED)

if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "DEVKITPRO environment variable is not set. Please set it to your devkitpro installation path.")
endif()

if(NOT DEFINED CONSOLE)
    set(CONSOLE "wii")
elseif(CONSOLE STREQUAL "gamecube")
    set(CONSOLE "cube")
endif()

set(LIBOGC_DIR $ENV{DEVKITPRO}/libogc)

set(LOVEPOWER_SRC
    src/main.cpp

)

add_executable(LOVEPower ${LOVEPOWER_SRC})
include_directories(${LIBOGC_DIR}/include)
target_include_directories(LOVEPower PRIVATE
    ${LIBOGC_DIR}/src
)
target_link_directories(LOVEPower PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${LIBOGC_DIR}/lib/${CONSOLE}
)

target_link_libraries(LOVEPower
    lua
)
