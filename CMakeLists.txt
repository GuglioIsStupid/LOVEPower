cmake_minimum_required(VERSION 3.16)
project(LOVEPower VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -lm")

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

set(LOVE_SRC_MODULES_EVENT
    src/modules/event/Event.cpp
    src/modules/event/Event.h

    src/modules/event/wrap_Event.cpp
    src/modules/event/wrap_Event.h
    src/modules/event/wrap_Event.lua

    src/modules/event/sdl/Event.cpp
    src/modules/event/sdl/Event.h
)

set(LOVE_SRC_MODULES_SOUND
    src/modules/sound/Decoder.cpp
    src/modules/sound/Decoder.h
    src/modules/sound/Sound.cpp
    src/modules/sound/Sound.h
    src/modules/sound/SoundData.cpp
    src/modules/sound/SoundData.h

    src/modules/sound/lullaby/CoreAudioDecoder.cpp
    src/modules/sound/lullaby/CoreAudioDecoder.h
    src/modules/sound/lullaby/FLACDecoder.cpp
    src/modules/sound/lullaby/FLACDecoder.h
    src/modules/sound/lullaby/GmeDecoder.cpp
    src/modules/sound/lullaby/GmeDecoder.h
    #src/modules/sound/lullaby/ModPlugDecoder.cpp
    #src/modules/sound/lullaby/ModPlugDecoder.h
    src/modules/sound/lullaby/Mpg123Decoder.cpp
    src/modules/sound/lullaby/Mpg123Decoder.h
    src/modules/sound/lullaby/Sound.cpp
    src/modules/sound/lullaby/Sound.h
    src/modules/sound/lullaby/VorbisDecoder.cpp
    src/modules/sound/lullaby/VorbisDecoder.h
    src/modules/sound/lullaby/WaveDecoder.cpp
    src/modules/sound/lullaby/WaveDecoder.h

    src/modules/sound/wrap_Decoder.cpp
    src/modules/sound/wrap_Decoder.h
    src/modules/sound/wrap_Sound.cpp
    src/modules/sound/wrap_Sound.h
    src/modules/sound/wrap_SoundData.cpp
    src/modules/sound/wrap_SoundData.h
    src/modules/sound/wrap_SoundData.lua
)

set(LOVE_SRC_MODULES_AUDIO
    src/modules/audio/Audio.cpp
    src/modules/audio/Audio.h
    src/modules/audio/Effect.cpp
    src/modules/audio/Effect.h
    src/modules/audio/Filter.cpp
    src/modules/audio/Filter.h
    src/modules/audio/RecordingDevice.cpp
    src/modules/audio/RecordingDevice.h
    src/modules/audio/Source.cpp
    src/modules/audio/Source.h

    src/modules/audio/null/Audio.cpp
    src/modules/audio/null/Audio.h
    src/modules/audio/null/RecordingDevice.cpp
    src/modules/audio/null/RecordingDevice.h
    src/modules/audio/null/Source.cpp
    src/modules/audio/null/Source.h

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
    src/modules/data/wrap_DataModule.cpp
    src/modules/data/wrap_DataModule.h
    src/modules/data/wrap_DataView.cpp
    src/modules/data/wrap_DataView.h
)

set(LOVE_SRC_MODULES_TIMER
    src/modules/timer/Timer.cpp
    src/modules/timer/Timer.h

    src/modules/timer/wrap_Timer.cpp
    src/modules/timer/wrap_Timer.h
)

set(LOVE_SRC_MODULES_KEYBOARD
    src/modules/keyboard/Keyboard.cpp
    src/modules/keyboard/Keyboard.h

    src/modules/keyboard/wrap_Keyboard.cpp
    src/modules/keyboard/wrap_Keyboard.h

    src/modules/keyboard/sdl/Keyboard.cpp
    src/modules/keyboard/sdl/Keyboard.h
)

set(LOVE_SRC_MODULES_MOUSE
    src/modules/mouse/Cursor.cpp
    src/modules/mouse/Cursor.h
    src/modules/mouse/Mouse.h

    src/modules/mouse/wrap_Cursor.cpp
    src/modules/mouse/wrap_Cursor.h
    src/modules/mouse/wrap_Mouse.cpp
    src/modules/mouse/wrap_Mouse.h

    src/modules/mouse/sdl/Cursor.cpp
    src/modules/mouse/sdl/Cursor.h
    src/modules/mouse/sdl/Mouse.cpp
    src/modules/mouse/sdl/Mouse.h
)

set(LOVE_SRC_MODULES_JOYSTICK
    src/modules/joystick/Joystick.cpp
    src/modules/joystick/Joystick.h
    src/modules/joystick/JoystickModule.h

    src/modules/joystick/sdl/Joystick.cpp
    src/modules/joystick/sdl/Joystick.h
    src/modules/joystick/sdl/JoystickModule.cpp
    src/modules/joystick/sdl/JoystickModule.h

    src/modules/joystick/wrap_Joystick.cpp
    src/modules/joystick/wrap_Joystick.h
    src/modules/joystick/wrap_JoystickModule.cpp
    src/modules/joystick/wrap_JoystickModule.h
)

set(LOVE_SRC_MODULES_THREAD
    src/modules/thread/Channel.cpp
    src/modules/thread/Channel.h
    src/modules/thread/LuaThread.cpp
    src/modules/thread/LuaThread.h
    src/modules/thread/Thread.h
    src/modules/thread/ThreadModule.cpp
    src/modules/thread/ThreadModule.h
    src/modules/thread/threads.cpp
    src/modules/thread/threads.h

    src/modules/thread/sdl/Thread.cpp
    src/modules/thread/sdl/Thread.h
    src/modules/thread/sdl/threads.cpp
    src/modules/thread/sdl/threads.h

    src/modules/thread/wrap_Channel.cpp
    src/modules/thread/wrap_Channel.h
    src/modules/thread/wrap_LuaThread.cpp
    src/modules/thread/wrap_LuaThread.h
    src/modules/thread/wrap_ThreadModule.cpp
    src/modules/thread/wrap_ThreadModule.h
)

set(LOVE_SRC_MODULES_IMAGE
    src/modules/image/CompressedImageData.cpp
    src/modules/image/CompressedImageData.h
    src/modules/image/CompressedSlice.cpp
    src/modules/image/CompressedSlice.h
    src/modules/image/FormatHandler.cpp
    src/modules/image/FormatHandler.h
    src/modules/image/Image.cpp
    src/modules/image/Image.h
    src/modules/image/ImageData.cpp
    src/modules/image/ImageData.h
    src/modules/image/ImageDataBase.cpp
    src/modules/image/ImageDataBase.h

    src/modules/image/wrap_CompressedImageData.cpp
    src/modules/image/wrap_CompressedImageData.h
    src/modules/image/wrap_Image.cpp
    src/modules/image/wrap_Image.h
    src/modules/image/wrap_ImageData.cpp
    src/modules/image/wrap_ImageData.h
    src/modules/image/wrap_ImageData.lua

    src/modules/image/magpie/ASTCHandler.cpp
    src/modules/image/magpie/ASTCHandler.h
    src/modules/image/magpie/ddsHandler.cpp
    src/modules/image/magpie/ddsHandler.h
    src/modules/image/magpie/EXRHandler.cpp
    src/modules/image/magpie/EXRHandler.h
    src/modules/image/magpie/KTXHandler.cpp
    src/modules/image/magpie/KTXHandler.h
    src/modules/image/magpie/PKMHandler.cpp
    src/modules/image/magpie/PKMHandler.h
    src/modules/image/magpie/PNGHandler.cpp
    src/modules/image/magpie/PNGHandler.h
    src/modules/image/magpie/PVRHandler.cpp
    src/modules/image/magpie/PVRHandler.h
    src/modules/image/magpie/STBHandler.cpp
    src/modules/image/magpie/STBHandler.h
)

set(LOVE_SRC_MODULES_FONT
    src/modules/font/BMFontRasterizer.cpp
    src/modules/font/BMFontRasterizer.h
    src/modules/font/Font.cpp
    src/modules/font/Font.h
    src/modules/font/GlyphData.cpp
    src/modules/font/GlyphData.h
    src/modules/font/ImageRasterizer.cpp
    src/modules/font/ImageRasterizer.h
    src/modules/font/Rasterizer.cpp
    src/modules/font/Rasterizer.h
    src/modules/font/TrueTypeRasterizer.cpp
    src/modules/font/TrueTypeRasterizer.h
    src/modules/font/Vera.ttf.h

    src/modules/font/freetype/Font.cpp
    src/modules/font/freetype/Font.h
    src/modules/font/freetype/TrueTypeRasterizer.cpp
    src/modules/font/freetype/TrueTypeRasterizer.h

    src/modules/font/wrap_Font.cpp
    src/modules/font/wrap_Font.h
    src/modules/font/wrap_GlyphData.cpp
    src/modules/font/wrap_GlyphData.h
    src/modules/font/wrap_Rasterizer.cpp
    src/modules/font/wrap_Rasterizer.h
)

set(LOVE_SRC_MODULES_GRAPHICS
    src/modules/graphics/Buffer.cpp
    src/modules/graphics/Buffer.h
    src/modules/graphics/Canvas.cpp
    src/modules/graphics/Canvas.h
    src/modules/graphics/Deprecations.cpp
    src/modules/graphics/Deprecations.h
    src/modules/graphics/depthstencil.cpp
    src/modules/graphics/depthstencil.h
    src/modules/graphics/Drawable.cpp
    src/modules/graphics/Drawable.h
    src/modules/graphics/Font.cpp
    src/modules/graphics/Font.h
    src/modules/graphics/Graphics.cpp
    src/modules/graphics/Graphics.h
    src/modules/graphics/Image.cpp
    src/modules/graphics/Image.h
    src/modules/graphics/Mesh.cpp
    src/modules/graphics/Mesh.h
    src/modules/graphics/ParticleSystem.cpp
    src/modules/graphics/ParticleSystem.h
    src/modules/graphics/Polyline.cpp
    src/modules/graphics/Polyline.h
    src/modules/graphics/Quad.cpp
    src/modules/graphics/Quad.h
    src/modules/graphics/Resource.h
    src/modules/graphics/Shader.cpp
    src/modules/graphics/Shader.h
    src/modules/graphics/ShaderStage.cpp
    src/modules/graphics/ShaderStage.h
    src/modules/graphics/SpriteBatch.cpp
    src/modules/graphics/SpriteBatch.h
    src/modules/graphics/StreamBuffer.cpp
    src/modules/graphics/StreamBuffer.h
    src/modules/graphics/Text.cpp
    src/modules/graphics/Text.h
    src/modules/graphics/Texture.cpp
    src/modules/graphics/Texture.h
    src/modules/graphics/vertex.cpp
    src/modules/graphics/vertex.h
    src/modules/graphics/Video.cpp
    src/modules/graphics/Video.h
    src/modules/graphics/Volatile.cpp
    src/modules/graphics/Volatile.h

    src/modules/graphics/opengl/Buffer.cpp
    src/modules/graphics/opengl/Buffer.h
    src/modules/graphics/opengl/Canvas.cpp
    src/modules/graphics/opengl/Canvas.h
    src/modules/graphics/opengl/FenceSync.cpp
    src/modules/graphics/opengl/FenceSync.h
    src/modules/graphics/opengl/Graphics.cpp
    src/modules/graphics/opengl/Graphics.h
    src/modules/graphics/opengl/Image.cpp
    src/modules/graphics/opengl/Image.h
    src/modules/graphics/opengl/OpenGL.cpp
    src/modules/graphics/opengl/OpenGL.h
    src/modules/graphics/opengl/Shader.cpp
    src/modules/graphics/opengl/Shader.h
    src/modules/graphics/opengl/ShaderStage.cpp
    src/modules/graphics/opengl/ShaderStage.h
    src/modules/graphics/opengl/StreamBuffer.cpp
    src/modules/graphics/opengl/StreamBuffer.h

    src/modules/graphics/wrap_Canvas.cpp
    src/modules/graphics/wrap_Canvas.h
    src/modules/graphics/wrap_Font.cpp
    src/modules/graphics/wrap_Font.h
    src/modules/graphics/wrap_Graphics.cpp
    src/modules/graphics/wrap_Graphics.h
    src/modules/graphics/wrap_Graphics.lua
    src/modules/graphics/wrap_GraphicsShader.lua
    src/modules/graphics/wrap_Image.cpp
    src/modules/graphics/wrap_Image.h
    src/modules/graphics/wrap_Mesh.cpp
    src/modules/graphics/wrap_Mesh.h
    src/modules/graphics/wrap_ParticleSystem.cpp
    src/modules/graphics/wrap_ParticleSystem.h
    src/modules/graphics/wrap_Quad.cpp
    src/modules/graphics/wrap_Quad.h
    src/modules/graphics/wrap_Shader.cpp
    src/modules/graphics/wrap_Shader.h
    src/modules/graphics/wrap_SpriteBatch.cpp
    src/modules/graphics/wrap_SpriteBatch.h
    src/modules/graphics/wrap_Text.cpp
    src/modules/graphics/wrap_Text.h
    src/modules/graphics/wrap_Texture.cpp
    src/modules/graphics/wrap_Texture.h
    src/modules/graphics/wrap_Video.cpp
    src/modules/graphics/wrap_Video.h
    src/modules/graphics/wrap_Video.lua
)

set(LOVE_SRC_MODULES_WINDOW
    src/modules/window/Window.cpp
    src/modules/window/Window.h

    src/modules/window/wrap_Window.cpp
    src/modules/window/wrap_Window.h

    src/modules/window/sdl/Window.cpp
    src/modules/window/sdl/Window.h
)

set(LOVE_SRC_MODULES_MATH
    src/modules/math/BezierCurve.cpp
    src/modules/math/BezierCurve.h
    src/modules/math/MathModule.cpp
    src/modules/math/MathModule.h
    src/modules/math/RandomGenerator.cpp
    src/modules/math/RandomGenerator.h
    src/modules/math/Transform.cpp
    src/modules/math/Transform.h

    src/modules/math/wrap_BezierCurve.cpp
    src/modules/math/wrap_BezierCurve.h
    src/modules/math/wrap_Math.cpp
    src/modules/math/wrap_Math.h
    src/modules/math/wrap_Math.lua
    src/modules/math/wrap_RandomGenerator.cpp
    src/modules/math/wrap_RandomGenerator.h
    src/modules/math/wrap_RandomGenerator.lua
    src/modules/math/wrap_Transform.cpp
    src/modules/math/wrap_Transform.h
)

set(LOVE_SRC_MODULES_VIDEO
    src/modules/video/Video.h
    src/modules/video/VideoStream.cpp
    src/modules/video/VideoStream.h

    src/modules/video/theora/OggDemuxer.cpp
    src/modules/video/theora/OggDemuxer.h
    src/modules/video/theora/TheoraVideoStream.cpp
    src/modules/video/theora/TheoraVideoStream.h
    src/modules/video/theora/Video.cpp
    src/modules/video/theora/Video.h

    src/modules/video/wrap_Video.cpp
    src/modules/video/wrap_Video.h
    src/modules/video/wrap_VideoStream.cpp
    src/modules/video/wrap_VideoStream.h
)

set(LOVE_FULL
    ${LOVE_SRC_COMMON}
    ${LOVE_SRC_MODULES_LOVE}
    ${LOVE_SRC_MODULES_EVENT}
    ${LOVE_SRC_MODULES_DATA}
    ${LOVE_SRC_MODULES_AUDIO}
    ${LOVE_SRC_MODULES_FILESYSTEM}
    ${LOVE_SRC_MODULES_FONT}
    ${LOVE_SRC_MODULES_GRAPHICS}
    ${LOVE_SRC_MODULES_TIMER}
    ${LOVE_SRC_MODULES_KEYBOARD}
    ${LOVE_SRC_MODULES_JOYSTICK}
    ${LOVE_SRC_MODULES_MOUSE}
    ${LOVE_SRC_MODULES_THREAD}
    ${LOVE_SRC_MODULES_IMAGE}
    ${LOVE_SRC_MODULES_WINDOW}
    ${LOVE_SRC_MODULES_MATH}
    ${LOVE_SRC_MODULES_VIDEO}
    ${LOVE_SRC_MODULES_SOUND}
)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/common)
include_directories(${CMAKE_SOURCE_DIR}/src/modules)
include_directories(${CMAKE_SOURCE_DIR}/src/libraries)

set(LUA_53_DIR ${CMAKE_SOURCE_DIR}/src/libraries/lua53)
set(DR_DIR ${CMAKE_SOURCE_DIR}/src/libraries/dr)
set(LZ4_DIR ${CMAKE_SOURCE_DIR}/src/libraries/lz4)
set(NOISE1234_DIR ${CMAKE_SOURCE_DIR}/src/libraries/noise1234)
set(STB_DIR ${CMAKE_SOURCE_DIR}/src/libraries/stb)
set(UTF8_DIR ${CMAKE_SOURCE_DIR}/src/libraries/utf8)
set(DDSPARSE_DIR ${CMAKE_SOURCE_DIR}/src/libraries/ddsparse)
set(LODEPNG_DIR ${CMAKE_SOURCE_DIR}/src/libraries/lodepng)
set(WUFF_DIR ${CMAKE_SOURCE_DIR}/src/libraries/Wuff)
set(GLSLANG_DIR ${CMAKE_SOURCE_DIR}/src/libraries/glslang)
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/src/libraries/glad)
set(XXHASH_DIR ${CMAKE_SOURCE_DIR}/src/libraries/xxHash)
#set(MODPLUG_DIR ${CMAKE_SOURCE_DIR}/src/libraries/modplug)

include_directories(
    ${LUA_53_DIR}
    ${DR_DIR}
    ${LZ4_DIR}
    ${NOISE1234_DIR}
    ${STB_DIR}
    ${UTF8_DIR}
    ${DDSPARSE_DIR}
    ${LODEPNG_DIR}
    ${GLSLANG_DIR}
    ${GLAD_DIR}
    ${XXHASH_DIR}
    ${WUFF_DIR}
    #${MODPLUG_DIR}

    ${SDL2_INCLUDE_DIRS}
)

file(GLOB DR_SOURCES ${DR_DIR}/*.h ${DR_DIR}/*.c)
file(GLOB LUA53_SOURCES ${LUA_53_DIR}/*.h ${LUA_53_DIR}/*.c)
file(GLOB LZ4_SOURCES ${LZ4_DIR}/*.h ${LZ4_DIR}/*.c)
file(GLOB NOISE1234_SOURCES ${NOISE1234_DIR}/*.h ${NOISE1234_DIR}/*.c)
file(GLOB STB_SOURCES ${STB_DIR}/*.h)
file(GLOB UTF8_SOURCES ${UTF8_DIR}/*.h)
file(GLOB DDSPARSE_SOURCES ${DDSPARSE_DIR}/*.h ${DDSPARSE_DIR}/*.cpp)
file(GLOB LODEPNG_SOURCES ${LODEPNG_DIR}/*.h ${LODEPNG_DIR}/*.cpp)
file(GLOB GLSLANG_SOURCES ${GLSLANG_DIR}/*.h ${GLSLANG_DIR}/*.cpp)
file(GLOB GLAD_SOURCES ${GLAD_DIR}/*.h ${GLAD_DIR}/*.c)
file(GLOB XXHASH_SOURCES ${XXHASH_DIR}/*.h ${XXHASH_DIR}/*.c)
file(GLOB WUFF_SOURCES ${WUFF_DIR}/*.h ${WUFF_DIR}/*.c)
#file(GLOB MODPLUG_SOURCES ${MODPLUG_DIR}/*.h ${MODPLUG_DIR}/*.c ${MODPLUG_DIR}/*.cpp)

list(APPEND LOVE_LIB_SRC 
    ${DR_SOURCES} ${LUA53_SOURCES} ${LZ4_SOURCES} 
    ${NOISE1234_SOURCES} ${STB_SOURCES} ${UTF8_SOURCES} 
    ${DDSPARSE_SOURCES} ${GLSLANG_SOURCES} ${GLAD_SOURCES}
    ${XXHASH_SOURCES} ${LODEPNG_SOURCES} ${WUFF_SOURCES}
    #${MODPLUG_SOURCES}
)

set(SOURCES
    src/love.cpp
    ${LOVE_FULL}
)

add_definitions(-DLOVE_NOMPG123 -DLOVE_NO_MODPLUG)

add_executable(${PROJECT_NAME} ${SOURCES} ${LOVE_LIB_SRC})

set(LIBOGC2_LUAJIT_LIB ${DEVKITPRO}/libogc2/wii/lib/libluajit.a)
include_directories(${DEVKITPRO}/libogc2/wii/include/luajit)

message(STATUS "Using PORTLIBS_PATH: ${DEVKITPRO}/portlibs")
set(WII_PHYSFS_LIB ${DEVKITPRO}/portlibs/wii/lib/libphysfs.a)
include_directories(${DEVKITPRO}/portlibs/wii/include)

set(PPC_ZLIB_LIB ${DEVKITPRO}/portlibs/ppc/lib/libz.a)
include_directories(${DEVKITPRO}/portlibs/ppc/include)

set(PPC_FREETYPE_LIB ${DEVKITPRO}/portlibs/ppc/lib/libfreetype.a)
include_directories(${DEVKITPRO}/portlibs/ppc/include/freetype2)

include_directories(${DEVKITPPC}/powerpc-eabi/include)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${EXTRA_LIBS} 
    ${LIBOGC2_LUAJIT_LIB} 
    ${SDL2_LIBRARIES} 
    ${WII_PHYSFS_LIB}
    ${PPC_ZLIB_LIB}
    ${PPC_FREETYPE_LIB}
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
