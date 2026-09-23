#include "math.hpp"
#include "classes/RandomGenerator.hpp"
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace math {
        #pragma region Random
        love::math::RandomGenerator::Seed makeSeed(uint16_t low, uint16_t high) {
            love::math::RandomGenerator::Seed s {};
            s.parts.low = low;
            s.parts.high = high;
            return s;
        }

        void __init(sol::state &luastate) {
        }

        void setRandomSeed_uint32(uint32_t seed) {
            love::math::RandomGenerator::Seed s {};
            s.b32 = seed;
            g_rng.setSeed(s);
        }

        void setRandomSeed_parts(uint16_t low, uint16_t high) {
            g_rng.setSeed(makeSeed(low, high));
        }

        void setRandomState(const std::string &state) {
            g_rng.setState(state);
        }

        std::tuple<uint16_t, uint16_t> getRandomSeed() {
            auto s = g_rng.getSeed();
            return {
                s.parts.low, 
                s.parts.high 
            };
        }

        std::string getRandomState() {
            return g_rng.getState();
        }

        double random() {
            return g_rng.random();
        }

        double random_max(double max) {
            return g_rng.random(max);
        }

        double random_min_max(double min, double max) {
            return g_rng.random(min, max);
        }

        #pragma region Color
        std::tuple<int16_t, int16_t, int16_t, int16_t> colorToBytes_double4(double r, double g, double b, double a) {
            int16_t r_int16_t = static_cast<int16_t>(r * 255);
            int16_t g_int16_t = static_cast<int16_t>(g * 255);
            int16_t b_int16_t = static_cast<int16_t>(b * 255);
            int16_t a_int16_t = static_cast<int16_t>(a * 255);

            return std::make_tuple(r_int16_t, g_int16_t, b_int16_t, a_int16_t);
        }
        std::tuple<int16_t, int16_t, int16_t> colorToBytes_double3(double r, double g, double b) {
            int16_t r_int16_t = static_cast<int16_t>(r * 255);
            int16_t g_int16_t = static_cast<int16_t>(g * 255);
            int16_t b_int16_t = static_cast<int16_t>(b * 255);

            return std::make_tuple(r_int16_t, g_int16_t, b_int16_t);
        }

        std::tuple<double, double, double, double> colorFromBytes_int4(int16_t r, int16_t g, int16_t b, int16_t a) {
            double r_double_t = static_cast<double>(r / 255);
            double g_double_t = static_cast<double>(g / 255);
            double b_double_t = static_cast<double>(b / 255);
            double a_double_t = static_cast<double>(a / 255);

            return std::make_tuple(r_double_t, g_double_t, b_double_t, a_double_t);
        }
        std::tuple<double, double, double> colorFromBytes_int3(int16_t r, int16_t g, int16_t b) {
            double r_double_t = static_cast<double>(r / 255);
            double g_double_t = static_cast<double>(g / 255);
            double b_double_t = static_cast<double>(b / 255);

            return std::make_tuple(r_double_t, g_double_t, b_double_t);
        }
    }
}

int luaopen_love_math(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["math"] = luastate.create_table_with(
        "setRandomSeed", sol::overload(
            static_cast<void(*)(uint32_t)>(&love::math::setRandomSeed_uint32),
            static_cast<void(*)(uint16_t, uint16_t)>(&love::math::setRandomSeed_parts)
        ),
        "setRandomState", love::math::setRandomState,
        "getRandomSeed", love::math::getRandomSeed,
        "getRandomState", love::math::getRandomState,
        "random", sol::overload(
            static_cast<double(*)()>(&love::math::random),
            static_cast<double(*)(double)>(&love::math::random_max),
            static_cast<double(*)(double, double)>(&love::math::random_min_max)
        ),

        "colorToBytes", sol::overload(
            love::math::colorToBytes_double3,
            love::math::colorToBytes_double4
        ),
        "colorFromBytes", sol::overload(
            love::math::colorFromBytes_int3,
            love::math::colorFromBytes_int4
        )
    );

    return 1;
}
