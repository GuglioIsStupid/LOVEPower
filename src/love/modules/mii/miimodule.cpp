#include "miimodule.hpp"

#include <sol/sol.hpp>

namespace love {
    namespace mii {
        void __init(sol::state &luastate) {
            sol::table mii = luastate.create_table(
            );

            luastate["love"]["mii"] = mii; 
        }
    }
}