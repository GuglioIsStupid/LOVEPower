#ifndef LOVE_CONFIG_H
#define LOVE_CONFIG_H

#if defined(HW_DOL)
#   define LOVE_GAMECUBE 1
#endif
#if defined(HW_RVL)
#   define LOVE_WII 1
#endif

#define LOVE_BIG_ENDIAN 1

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef LOVE_UNUSED
#	define LOVE_UNUSED(x) (void)sizeof(x)
#endif

#define LOVE_WARN_UNUSED

#define LOVE_ENABLE_LOVE
#define LOVE_ENABLE_AUDIO
#define LOVE_ENABLE_DATA
#define LOVE_ENABLE_EVENT
#define LOVE_ENABLE_FILESYSTEM
#define LOVE_ENABLE_FONT
#define LOVE_ENABLE_GRAPHICS
#define LOVE_ENABLE_IMAGE
#define LOVE_ENABLE_JOYSTICK
#define LOVE_ENABLE_KEYBOARD
#define LOVE_ENABLE_MATH
#define LOVE_ENABLE_MOUSE
#define LOVE_ENABLE_PHYSICS
#define LOVE_ENABLE_SOUND
#define LOVE_ENABLE_SYSTEM
#define LOVE_ENABLE_THREAD
#define LOVE_ENABLE_TIMER
#define LOVE_ENABLE_TOUCH
#define LOVE_ENABLE_VIDEO
#define LOVE_ENABLE_WINDOW

//#define LOVE_ENABLE_ENET
//#define LOVE_ENABLE_LUASOCKET
//#define LOVE_ENABLE_LUA51

#if !defined(LOVE_WII) && !defined(LOVE_GAMECUBE)
#  error "Either LOVE_WII or LOVE_GAMECUBE must be defined."
#endif

#endif // LOVE_CONFIG_H