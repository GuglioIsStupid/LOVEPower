#pragma once

#include <string>
#include <tuple>

#include <sol/sol.hpp>

namespace love {
    void UNUSED();
    void UNUSED(...);
    void logError(const std::string &msg);
    
    inline std::string _console                            = "Wii";
    inline std::string _os                                 = "Wii";
    inline std::tuple<int, int, int, std::string> _version = std::make_tuple(0, 1, 0, "UNKNOWN");
    inline bool _deprecationOutput                         = false;

    bool hasDeprecationOutput();
    void setDeprecationOutput(bool deprecationOutput);
    std::tuple<int, int, int, std::string> getVersion();

    int initialize(int argc, char** argv);
    int exit();
}

int luaopen_love(lua_State *L);
int luaopen_love_nogame(lua_State *L);
int luaopen_love_arg(lua_State *L);
int luaopen_love_callbacks(lua_State *L);
int luaopen_love_boot(lua_State *L);
int luaopen_love_jitsetup(lua_State *L);
