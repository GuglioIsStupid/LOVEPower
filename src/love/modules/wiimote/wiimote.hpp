#pragma once

#include <sol/sol.hpp>
#include <vector>
#include <string>

#include "classes/wiimoteController.hpp"

namespace love {
    namespace wiimote {
        void    __init(sol::state &luastate);
        void    __registerTypes(sol::state &luastate);

        WiimoteController* getWiimote(int index);
        void    update();
    }
}