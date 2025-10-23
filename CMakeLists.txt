cmake_minimum_required(VERSION 3.16)
project(LOVEPower VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 IMPORTED_TARGET sdl2)
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

set(SOURCES
    src/love.cpp
)

set(LOVE_LIB_SRC
    ${LOVE_SRC_COMMON}
)

add_executable(${PROJECT_NAME} ${SOURCES} ${LOVE_LIB_SRC})

include_directories("$DEVKITPRO/libogc2/wii/include")

list(APPEND EXTRA_LIBS PkgConfig::SDL2)

target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTRA_LIBS})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND elf2dol.exe ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dol
    COMMENT "Converting .elf to .dol using elf2dol.exe"
)

#add_custom_target(run
#    COMMAND wiiload ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dol
#    DEPENDS ${PROJECT_NAME}
#    COMMENT "Uploading to Wii"
#)
