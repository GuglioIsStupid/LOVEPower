#include "math.hpp"
#include "classes/randomGenerator.hpp"

namespace love {
    namespace math {
        love::math::RandomGenerator::Seed makeSeed(uint16_t low, uint16_t high) {
            love::math::RandomGenerator::Seed s {};
            s.parts.low = low;
            s.parts.high = high;
            return s;
        }

        void __init(sol::state &luastate) {
            sol::table math = luastate.create_table_with(
                "setRandomSeed", sol::overload(
                    setRandomSeed_uint32,
                    setRandomSeed_parts
                ),
                "random", sol::overload(
                    random,
                    random_max,
                    random_min_max
                ),
                "setRandomState", setRandomState,
                "getRandomSeed", getRandomSeed,
                "getRandomState", getRandomState
            );

            luastate["love"]["math"] = math;
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
    }
}
