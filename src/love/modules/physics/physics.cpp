#include "physics.hpp"

#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace physics {
        void __init(sol::state &luastate) {
            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            
        }
    }
}

int luaopen_love_physics(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["physics"] = luastate.create_table(
    );

    return 1;
}