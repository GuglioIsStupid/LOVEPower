#pragma once

#include <sol/sol.hpp>

#include "classes/RandomGenerator.hpp"
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace math {
        inline love::math::RandomGenerator g_rng;
        love::math::RandomGenerator::Seed makeSeed(uint16_t low, uint16_t high);

        void __init(sol::state &luastate);

        void setRandomSeed_uint32(uint32_t seed);
        void setRandomSeed_parts(uint16_t low, uint16_t high);

        void setRandomState(const std::string &state);
        std::tuple<uint16_t, uint16_t> getRandomSeed();
        std::string getRandomState();

        double random();
        double random_max(double max);
        double random_min_max(double min, double max);

        std::tuple<uint16_t, uint16_t> getRandomSeed();
        std::string getRandomState();
    }
}

int luaopen_love_math(lua_State *L);
