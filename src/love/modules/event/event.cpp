#include <sol/sol.hpp>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <grrlib.h>

#include "event.hpp"

namespace love {
    namespace event {
        std::vector<event_t> events;
        std::vector<event_t>::iterator currentEvent = events.end();

        void __init(sol::state & luastate) {
            return;
        }

        void pump(sol::this_state lua) {
            currentEvent = events.begin();
        }

        event_t poll(sol::this_state lua) {
            if (currentEvent == events.end()) {
                events.clear();

                currentEvent = events.end();
                return std::make_tuple(sol::lua_nil, sol::lua_nil, sol::lua_nil, sol::lua_nil, sol::lua_nil, sol::lua_nil, sol::lua_nil);
            } else {
                return *currentEvent++;
            }
        }

        void __pushEvent(sol::this_state lua, const char *eventName, sol::object a = sol::lua_nil, sol::object b = sol::lua_nil, sol::object c = sol::lua_nil, sol::object d = sol::lua_nil, sol::object e = sol::lua_nil, sol::object f = sol::lua_nil) {
            push(sol::make_object(lua, eventName), a, b, c, d, e, f, lua);
        }

        void push(sol::object name, sol::object a, sol::object b, sol::object c, sol::object d, sol::object e, sol::object f, sol::this_state s) {
            events.push_back(std::make_tuple(name, a, b, c, d, e, f));
        }

        void quit(sol::this_state lua) {
            __pushEvent(lua, "quit");
        }
    }
}
