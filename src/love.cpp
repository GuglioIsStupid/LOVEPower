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

#include <fstream>
#include <iostream>

template class std::vector<std::string>;

static std::string print_log_buffer = "";

static int custom_print(lua_State *L) {
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char *str = lua_tolstring(L, i, NULL);
        print_log_buffer += str;
        if (i < n) {
            print_log_buffer += "\t";
        }
        lua_pop(L, 1);
    }
    print_log_buffer += "\n";
    return 0;
}

static void get_app_arguments(int argc, char** argv, int &new_argc, char** &new_argv)
{
    argv[0] = const_cast<char*>("LOVEPower/game/");
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
	retval = 0;
	DoneAction done = DONE_QUIT;

	luaL_openlibs(L);

	// Override Lua's print function
	lua_pushcfunction(L, custom_print);
	lua_setglobal(L, "print");

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

	#ifndef LUA_OK
	#define LUA_OK 0
	#endif
	if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
		std::ofstream logfile("love_log_error.txt", std::ios::app);
		logfile << "Error calling 'require(\"love.boot\")': " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return DONE_QUIT;
	}
	
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
	get_app_arguments(argc, argv, argc, argv);

	int retval = 0;
	DoneAction done = DONE_QUIT;
	love::Variant restartvalue;

	do {
		done = runlove(argc, argv, retval, restartvalue);
	} while (done == DONE_RESTART);

	std::ofstream print_log_file("love_print_out.txt", std::ios::app);
	print_log_file << "----- PRINT LOG -----\n" << print_log_buffer;
	print_log_file.close();
	
    return retval;
}
