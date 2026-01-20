#pragma once

#include <cstdint>
#include <string>

namespace love {
    namespace math {

        uint32_t wangHash32(uint32_t key);

        struct RandomGenerator {
            union Seed {
                uint32_t b32;
                struct {
                    uint16_t low;
                    uint16_t high;
                } parts;
            };

            RandomGenerator();
            explicit RandomGenerator(Seed s);

            void setSeed(Seed s);
            Seed getSeed() const;

            uint32_t rand();

            double random();
            double random(double max);
            double random(double min, double max);

            double randomNormal(double stddev);

            void setState(const std::string &statestr);
            std::string getState() const;

            private:
                Seed seed;
                Seed rng_state;
                double last_randomnormal;
        };

    }
}
