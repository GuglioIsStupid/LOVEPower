#pragma once

#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace timer {
        void    __init(sol::state &luastate);

        double step();
        void sleep(double seconds);
        double getDelta();
        int getFPS();
        double getAverageDelta();
        double getTime();

        extern double _currTime;
        extern double _prevTime;
        extern double _prevFpsUpdate;

        extern int _fps;
        extern double _averageDelta;
        extern double _fpsUpdateFrequency;
        extern int _frames;
        extern double _dt;
    }
}

int luaopen_love_timer(lua_State *L);