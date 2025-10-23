#include "wrap_Audio.h"
#include "filesystem/wrap_Filesystem.h"

#include "common/runtime.h"

// C++
#include <iostream>
#include <cmath>

namespace love
{
namespace audio
{

/* #define instance() (Module::getInstance<Audio>(Module::M_AUDIO)) */

static const luaL_Reg functions[] =
{
	{ 0, 0 }
};

static const lua_CFunction types[] =
{
	0
};

extern "C" int luaopen_love_audio(lua_State *L)
{
    /* Audio *inst = instance();

    if (inst == nullptr)
    {
        // idk
    }
    else
        inst->retain(); */

    WrappedModule w;
    w.module = nullptr; //inst;
    w.name = "audio";
    /* w.type = &Audio::type; */
    w.functions = functions;
    w.types = types;

    int n = luax_register_module(L, w);

    return n;
}

} // love
} // love