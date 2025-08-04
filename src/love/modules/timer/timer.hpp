#pragma once

#include <sol/sol.hpp>

namespace love {
    namespace timer {
        void    __init(sol::state &luastate);

        void    sleep(float ms);
        double  step();
        double  getFPS();
    }
}