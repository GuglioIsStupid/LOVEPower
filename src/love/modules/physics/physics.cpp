#include "physics.hpp"

#include <sol/sol.hpp>

namespace love {
    namespace physics {
        void __init(sol::state &luastate) {
            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            
        }
    }
}