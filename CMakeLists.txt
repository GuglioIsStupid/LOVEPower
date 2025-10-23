cmake_minimum_required(VERSION 3.16)
project(LOVEPower VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(OPENGL IMPORTED_TARGET opengl)

include(GNUInstallDirs)

set(EXTRA_LIBS)
list(APPEND EXTRA_LIBS opengx)

set(LOVE_SRC_COMMON
    src/common/config.h
    src/common/int.h
    src/common/runtime.cpp
    src/common/runtime.h
    src/common/version.h
)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/common)

set(LUA_53_DIR ${CMAKE_SOURCE_DIR}/src/libraries/lua53)
set(DR_DIR ${CMAKE_SOURCE_DIR}/src/libraries/dr)
set(LZ4_DIR ${CMAKE_SOURCE_DIR}/src/libraries/lz4)
set(NOISE1234_DIR ${CMAKE_SOURCE_DIR}/src/libraries/noise1234)
set(STB_DIR ${CMAKE_SOURCE_DIR}/src/libraries/stb)
set(UTF8_DIR ${CMAKE_SOURCE_DIR}/src/libraries/utf8)

include_directories(
    ${LUA_53_DIR}
    ${DR_DIR}
    ${LZ4_DIR}
    ${NOISE1234_DIR}
    ${STB_DIR}
    ${UTF8_DIR}
    ${SDL2_INCLUDE_DIRS}
)

file(GLOB DR_SOURCES ${DR_DIR}/*.h ${DR_DIR}/*.c)
file(GLOB LUA53_SOURCES ${LUA_53_DIR}/*.h ${LUA_53_DIR}/*.c)
file(GLOB LZ4_SOURCES ${LZ4_DIR}/*.h ${LZ4_DIR}/*.c)
file(GLOB NOISE1234_SOURCES ${NOISE1234_DIR}/*.h ${NOISE1234_DIR}/*.c)
file(GLOB STB_SOURCES ${STB_DIR}/*.h)
file(GLOB UTF8_SOURCES ${UTF8_DIR}/*.h)

list(APPEND LOVE_LIB_SRC ${DR_SOURCES} ${LUA53_SOURCES} ${LZ4_SOURCES} ${NOISE1234_SOURCES} ${STB_SOURCES} ${UTF8_SOURCES})

set(SOURCES
    src/love.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${LOVE_LIB_SRC})

set(LIBOGC2_LUAJIT_LIB ${DEVKITPRO}/libogc2/wii/lib/libluajit.a)
include_directories(${DEVKITPRO}/libogc2/wii/include/luajit)

target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTRA_LIBS} ${LIBOGC2_LUAJIT_LIB} ${SDL2_LIBRARIES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND elf2dol.exe ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dol
    COMMENT "Converting .elf to .dol using elf2dol.exe"
)

# add_custom_target(run
#     COMMAND wiiload ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dol
#     DEPENDS ${PROJECT_NAME}
#     COMMENT "Uploading to Wii"
# )
