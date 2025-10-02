#include <ogc/lwp_watchdog.h>
#include <sys/unistd.h>
#include <cstdint>
#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}
#include "timer.hpp"

namespace love {
    namespace timer {
        double _currTime = 0.0;
        double _prevTime = 0.0;
        double _prevFpsUpdate = 0.0;

        int _fps = 0;
        double _averageDelta = 0.0;
        double _fpsUpdateFrequency = 1.0;
        int _frames = 0;
        double _dt = 0.0;
        
        void __init(sol::state &luastate) {
            _prevFpsUpdate = _currTime = getTime();
        }

        double step() {
            _frames++;

            _prevTime = _currTime;
            _currTime = getTime();

            _dt = _currTime - _prevTime;

            double timeSinceLast = _currTime - _prevFpsUpdate;
            if (timeSinceLast > _fpsUpdateFrequency) {
                _fps = int((_frames/timeSinceLast) + 0.5);
                _averageDelta = timeSinceLast / _frames;
                _prevFpsUpdate = _currTime;
                _frames = 0;
            }

            return _dt;
        }

        void sleep(double seconds) {
            if (seconds >= 0)
                usleep((useconds_t)(seconds * 1e6));
        }

        double getDelta() {
            return _dt;
        }

        int getFPS() {
            return _fps;
        }

        double getAverageDelta() {
            return _averageDelta;
        }

        double getTime() {
            static const u64 start = gettime();
            const u64 now = gettime();
            const u64 diff = now - start;

            return (double)ticks_to_microsecs(diff) / 1.0e6;
        }
    }   
}

int luaopen_love_timer(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["timer"] = luastate.create_table_with(
        "step", &love::timer::step,
        "sleep", &love::timer::sleep,
        "getDelta", &love::timer::getDelta,
        "getFPS", &love::timer::getFPS,
        "getAverageDelta", &love::timer::getAverageDelta,
        "getTime", &love::timer::getTime
    );

    return 1;
}