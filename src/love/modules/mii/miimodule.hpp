#pragma once

#include <sol/sol.hpp>
#include "mii.h"

namespace love {
    namespace mii {
        void    __init(sol::state &luastate);
        void    __registerTypes(sol::state &luastate);

        sol::object getMiis(sol::state &luastate);
    }
}