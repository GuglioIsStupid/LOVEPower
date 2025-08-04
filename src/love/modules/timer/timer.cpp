#include <ogc/lwp_watchdog.h>
#include <sys/unistd.h>
#include <cstdint>
#include <sol/sol.hpp>
// Header
#include "timer.hpp"

namespace {
    std::uint64_t lastTime;
	double deltaTime = 0.0;

	std::uint64_t lastFrameTime;
	double fps = 0.0;
	double avgDelta = 0.0;
	double frames = 0.0;
}

namespace love {
    namespace timer {
        void __init(sol::state &luastate) {
            lastTime = gettime();
            lastFrameTime = lastTime;
        }

        void sleep(float ms) {
            if (ms <= 0) {
                return;
            }
            usleep((static_cast<unsigned int>(ms * 1000.0)));
        }

        double step() {
            const std::uint64_t curTime = gettime();

            const double secondsSinceLastFPSUpdate = static_cast<double>(curTime - lastFrameTime) / (static_cast<double>(TB_TIMER_CLOCK) * 1000.0);
            deltaTime = static_cast<double>(curTime - lastTime) / (static_cast<double>(TB_TIMER_CLOCK) * 1000.0);

            frames++;

            if (secondsSinceLastFPSUpdate >= 1.0) {
                fps = static_cast<int>(frames);
                if (frames > 0.0) {
                    avgDelta = secondsSinceLastFPSUpdate / frames;
                } else {
                    avgDelta = 0.0;
                }

                frames = 0.0;
                lastFrameTime = curTime;
            }

            lastTime = curTime;
            return deltaTime;
        }

        double getFPS() {
            return fps;
        }
    }
}