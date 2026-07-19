#pragma once

#include <sol/sol.hpp>
#include <tuple>
#include <vector>

extern "C" {
    #include <lua.h>
}

namespace love {
    namespace event {
        using event_t = std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object>;

        extern std::vector<event_t> events;

        void __init(sol::state &luastate);

        void pump(sol::this_state lua);
        sol::variadic_results poll(sol::this_state lua);
        void push(sol::object name, sol::object a, sol::object b, sol::object c, sol::object d, sol::object e, sol::object f, sol::this_state s);

        void quit(sol::this_state lua);
    }
}

int luaopen_love_event(lua_State *L);