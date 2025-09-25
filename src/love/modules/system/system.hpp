#pragma once
#include <string>
extern "C" {
    #include <lua.h>
}
#include <sol/sol.hpp>

namespace love {
    namespace system {
        std::string   getOS();
        int           getProcessorCount();
        
        std::tuple<std::string, std::optional<int>, std::optional<int>> getPowerInfo();
    }
}

int luaopen_love_system(lua_State *L);