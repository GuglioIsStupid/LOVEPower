#pragma once

#include <sol/sol.hpp>
#include <tuple>
#include <vector>

namespace love {
    namespace event {
        typedef std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> event_t;

        extern std::vector<event_t> events;
        extern std::vector<event_t>::iterator currentEvent;

        void __init(sol::state &luastate);

        void pump(sol::this_state lua);
        event_t poll(sol::this_state lua);
        void push(sol::object name, sol::object a, sol::object b, sol::object c, sol::object d, sol::object e, sol::object f, sol::this_state s);
        void quit(sol::this_state lua);

    }
}
