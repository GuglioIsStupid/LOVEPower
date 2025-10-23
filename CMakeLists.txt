cmake_minimum_required(VERSION 3.16)
project(LOVEPower VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fexceptions")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(OPENGL IMPORTED_TARGET opengl)

include(GNUInstallDirs)

set(EXTRA_LIBS)
list(APPEND EXTRA_LIBS opengx)

set(LOVE_SRC_COMMON
    #src/common/android.cpp
	#src/common/android.h
	src/common/b64.cpp
	src/common/b64.h
	src/common/Color.h
	src/common/config.h
	src/common/Data.cpp
	src/common/Data.h
	src/common/delay.cpp
	src/common/delay.h
	src/common/deprecation.cpp
	src/common/deprecation.h
	src/common/EnumMap.h
	src/common/Exception.cpp
	src/common/Exception.h
	src/common/floattypes.cpp
	src/common/floattypes.h
	src/common/int.h
	src/common/math.h
	src/common/Matrix.cpp
	src/common/Matrix.h
	src/common/memory.cpp
	src/common/memory.h
	src/common/Module.cpp
	src/common/Module.h
	src/common/Object.cpp
	src/common/Object.h
	src/common/Optional.h
	src/common/pixelformat.cpp
	src/common/pixelformat.h
	src/common/Range.h
	src/common/Reference.cpp
	src/common/Reference.h
	src/common/runtime.cpp
	src/common/runtime.h
	src/common/Stream.cpp
	src/common/Stream.h
	src/common/StringMap.cpp
	src/common/StringMap.h
	src/common/types.cpp
	src/common/types.h
	src/common/utf8.cpp
	src/common/utf8.h
	src/common/Variant.cpp
	src/common/Variant.h
	#src/common/Vector.cpp # Vector.cpp is empty.
	src/common/Vector.h
	src/common/version.h
)

set(LOVE_SRC_MODULES_LOVE
    src/modules/love/love.cpp
    src/modules/love/love.h

    src/modules/love/arg.lua
    src/modules/love/callbacks.lua
    src/modules/love/boot.lua
    src/modules/love/jitsetup.lua
)

set(LOVE_SRC_MODULES_AUDIO
    src/modules/audio/wrap_Audio.cpp
    src/modules/audio/wrap_Audio.h
    src/modules/audio/wrap_RecordingDevice.cpp
    src/modules/audio/wrap_RecordingDevice.h
    src/modules/audio/wrap_Source.cpp
    src/modules/audio/wrap_Source.h
)

set(LOVE_SRC_MODULES_FILESYSTEM
    src/modules/filesystem/physfs/File.cpp
    src/modules/filesystem/physfs/File.h
    src/modules/filesystem/physfs/Filesystem.cpp
    src/modules/filesystem/physfs/Filesystem.h
    src/modules/filesystem/physfs/PhysfsIo.cpp
    src/modules/filesystem/physfs/PhysfsIo.h
    src/modules/filesystem/File.cpp
    src/modules/filesystem/File.h
    src/modules/filesystem/FileData.cpp
    src/modules/filesystem/FileData.h
    src/modules/filesystem/Filesystem.cpp
    src/modules/filesystem/Filesystem.h
    src/modules/filesystem/NativeFile.cpp
    src/modules/filesystem/NativeFile.h

    src/modules/filesystem/wrap_File.cpp
    src/modules/filesystem/wrap_File.h
    src/modules/filesystem/wrap_FileData.cpp
    src/modules/filesystem/wrap_FileData.h
    src/modules/filesystem/wrap_Filesystem.cpp
    src/modules/filesystem/wrap_Filesystem.h
    src/modules/filesystem/wrap_NativeFile.cpp
    src/modules/filesystem/wrap_NativeFile.h
)

set(LOVE_SRC_MODULES_DATA
    src/modules/data/ByteData.cpp
    src/modules/data/ByteData.h
    src/modules/data/CompressedData.cpp
    src/modules/data/CompressedData.h
    src/modules/data/Compressor.cpp
    src/modules/data/Compressor.h
    src/modules/data/DataModule.cpp
    src/modules/data/DataModule.h
    src/modules/data/DataStream.cpp
    src/modules/data/DataStream.h
    src/modules/data/DataView.cpp
    src/modules/data/DataView.h
    src/modules/data/HashFunction.cpp
    src/modules/data/HashFunction.h

    src/modules/data/wrap_ByteData.cpp
    src/modules/data/wrap_ByteData.h
    src/modules/data/wrap_CompressedData.cpp
    src/modules/data/wrap_CompressedData.h
    src/modules/data/wrap_Data.cpp
    src/modules/data/wrap_Data.h
    src/modules/data/wrap_Data.lua
    src/modules/data/wrap_DataView.cpp
    src/modules/data/wrap_DataView.h
    src/modules/data/wrap_DataModule.cpp
    src/modules/data/wrap_DataModule.h
)

set(LOVE_FULL
    ${LOVE_SRC_COMMON}
    ${LOVE_SRC_MODULES_LOVE}
    ${LOVE_SRC_MODULES_DATA}
    ${LOVE_SRC_MODULES_AUDIO}
    ${LOVE_SRC_MODULES_FILESYSTEM}
)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/common)
include_directories(${CMAKE_SOURCE_DIR}/src/modules)

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
file(GLOB PHYSFS_SOURCES ${CMAKE_SOURCE_DIR}/src/libraries/physfs/*.h ${CMAKE_SOURCE_DIR}/src/libraries/physfs/*.c)

list(APPEND LOVE_LIB_SRC ${DR_SOURCES} ${LUA53_SOURCES} ${LZ4_SOURCES} ${NOISE1234_SOURCES} ${STB_SOURCES} ${UTF8_SOURCES})

set(SOURCES
    src/love.cpp
    ${LOVE_FULL}
)

add_executable(${PROJECT_NAME} ${SOURCES} ${LOVE_LIB_SRC})

set(LIBOGC2_LUAJIT_LIB ${DEVKITPRO}/libogc2/wii/lib/libluajit.a)
include_directories(${DEVKITPRO}/libogc2/wii/include/luajit)

message(STATUS "Using PORTLIBS_PATH: ${DEVKITPRO}/portlibs")
set(WII_PHYSFS_LIB ${DEVKITPRO}/portlibs/wii/lib/libphysfs.a)
include_directories(${DEVKITPRO}/portlibs/wii/include)

set(PPC_ZLIB_LIB ${DEVKITPRO}/portlibs/ppc/lib/libz.a)
include_directories(${DEVKITPRO}/portlibs/ppc/include)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${EXTRA_LIBS} 
    ${LIBOGC2_LUAJIT_LIB} 
    ${SDL2_LIBRARIES} 
    ${WII_PHYSFS_LIB}
    ${PPC_ZLIB_LIB}
)

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
