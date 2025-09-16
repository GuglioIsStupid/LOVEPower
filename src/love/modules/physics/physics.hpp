#pragma once

#include <sol/sol.hpp>

namespace love {
    namespace physics {
        void __init(sol::state &luastate);
        void __registerTypes(sol::state &luastate);
    }
}