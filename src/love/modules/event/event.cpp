#include <sol/sol.hpp>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <cstdlib>
#include "event.hpp"
#include <ogc/system.h>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace event {
        std::vector<event_t> events;
        std::vector<event_t>::iterator currentEvent = events.end();

        static bool requestQuit = false;
        static bool requestReset = false;

        static void onReset(u32 reset, void* usrdata) {
            requestReset = true;
        }

        static void onPower(void) {
            requestQuit = true;
        }

        static void onWpadPower(int chan) {
            requestQuit = true;
        }

        void __init(sol::state & luastate) {
            SYS_SetResetCallback(onReset);
            SYS_SetPowerCallback(onPower);
            WPAD_SetPowerButtonCallback(onWpadPower);
        }

        void __pushEvent(sol::this_state lua, const char *eventName,
                         sol::object a = sol::lua_nil, sol::object b = sol::lua_nil,
                         sol::object c = sol::lua_nil, sol::object d = sol::lua_nil,
                         sol::object e = sol::lua_nil, sol::object f = sol::lua_nil) {
            push(sol::make_object(lua, eventName), a, b, c, d, e, f, lua);
        }

        void pump(sol::this_state lua) {
            currentEvent = events.begin();

            if (requestQuit || requestReset) {
                __pushEvent(lua, "quit");

                if (requestReset) {
                    SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
                    exit(0);
                } else {
                    SYS_ResetSystem(SYS_POWEROFF, 0, 0);
                    exit(0);
                }
            }
        }

        event_t poll(sol::this_state lua) {
            if (currentEvent == events.end()) {
                events.clear();
                currentEvent = events.end();
                return std::make_tuple(sol::lua_nil, sol::lua_nil, sol::lua_nil,
                                       sol::lua_nil, sol::lua_nil, sol::lua_nil, sol::lua_nil);
            } else {
                return *currentEvent++;
            }
        }

        void push(sol::object name, sol::object a, sol::object b,
                  sol::object c, sol::object d, sol::object e,
                  sol::object f, sol::this_state s) {
            events.push_back(std::make_tuple(name, a, b, c, d, e, f));
        }

        void quit(sol::this_state lua) {
            __pushEvent(lua, "quit");

            SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
            std::exit(0);
        }
    }
}

int luaopen_love_event(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["event"] = luastate.create_table_with(
        "pump", love::event::pump,
        "poll", love::event::poll,
        "push", love::event::push,
        "quit", love::event::quit
    );

    return 1;
}