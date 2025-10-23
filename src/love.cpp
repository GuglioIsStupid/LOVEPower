#include "common/version.h"
#include "common/runtime.h"
#include "common/Variant.h"
#include "modules/love/love.h"
#include <SDL.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <vector>

template class std::vector<std::string>;

static void get_app_arguments(int argc, char** argv, int &new_argc, char** &new_argv)
{
    // guh (this isn't needed for the wii or gamecube... i think?)
	// really the only argument would be the path to the .dol lmao
    return;
}

static int love_preload(lua_State *L, lua_CFunction f, const char *name)
{
    lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, f);
	lua_setfield(L, -2, name);
	lua_pop(L, 2);
	return 0;
}

enum DoneAction
{
    DONE_QUIT,
    DONE_RESTART
};

static DoneAction runlove(int argc, char** argv, int &retval, love::Variant &restartvalue)
{
    lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	love_preload(L, luaopen_love_jitsetup, "love.jitsetup");
	lua_getglobal(L, "require");
	lua_pushstring(L, "love.jitsetup");
	lua_call(L, 1, 0);

	love_preload(L, luaopen_love, "love");

	{
		lua_newtable(L);

		if (argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}

		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for (int i = 1; i < argc; i++)
		{
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
		}

		lua_setglobal(L, "arg");
	}

	lua_getglobal(L, "require");
	lua_pushstring(L, "love");
	lua_call(L, 1, 1);

	{
		lua_pushboolean(L, 1);
		lua_setfield(L, -2, "_exe");
	}

	love::luax_pushvariant(L, restartvalue);
	lua_setfield(L, -2, "restart");
	restartvalue = love::Variant();

	lua_pop(L, 1);

	lua_getglobal(L, "require");
	lua_pushstring(L, "love.boot");
	lua_call(L, 1, 1);

	lua_newthread(L);
	lua_pushvalue(L, -2);
	int stackpos = lua_gettop(L);
	int nres;
	while (love::luax_resume(L, 0, &nres) == LUA_YIELD)
#if LUA_VERSION_NUM >= 504
		lua_pop(L, nres);
#else
		lua_pop(L, lua_gettop(L) - stackpos);
#endif

	retval = 0;
	DoneAction done = DONE_QUIT;

	// if love.boot() returns "restart", we'll start up again after closing this
	// Lua state.
	int retidx = stackpos;
	if (!lua_isnoneornil(L, retidx))
	{
		if (lua_type(L, retidx) == LUA_TSTRING && strcmp(lua_tostring(L, retidx), "restart") == 0)
			done = DONE_RESTART;
		if (lua_isnumber(L, retidx))
			retval = (int) lua_tonumber(L, retidx);

		// Disallow userdata (love objects) from being referenced by the restart
		// value.
		if (retidx < lua_gettop(L))
			restartvalue = love::luax_checkvariant(L, retidx + 1, false);
	}

	lua_close(L);

	return done;
}

int main(int argc, char** argv)
{
    // guh

    int retval = 0;
	DoneAction done = DONE_QUIT;
	love::Variant restartvalue;

    do
    {
        done = runlove(argc, argv, retval, restartvalue);
    } while (done == DONE_RESTART);

    return retval;
}
