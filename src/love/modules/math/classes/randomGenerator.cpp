#include "RandomGenerator.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>

#define M_PI 3.14159265358979323846

namespace love {
    namespace math {
        uint32_t wangHash32(uint32_t key) {
            key = (key ^ 61) ^ (key >> 16);
            key = key + (key << 3);
            key = key ^ (key >> 4);
            key = key * 0x27d4eb2d;
            key = key ^ (key >> 15);
            return key;
        }

        RandomGenerator::RandomGenerator() : last_randomnormal(std::numeric_limits<double>::infinity()) {
            Seed s;
            s.b32 = 0x12345678u;
            setSeed(s);
        }

        RandomGenerator::RandomGenerator(Seed s) : last_randomnormal(std::numeric_limits<double>::infinity()) {
            setSeed(s);
        }

        void RandomGenerator::setSeed(Seed s) {
            seed = s;
            uint32_t v = s.b32;
            do {
                v = wangHash32(v);
            } while (v == 0);
            rng_state.b32 = v;
            last_randomnormal = std::numeric_limits<double>::infinity();
        }

        RandomGenerator::Seed RandomGenerator::getSeed() const {
            return seed;
        }

        uint32_t RandomGenerator::rand() {
            uint32_t s = rng_state.b32 + 0x9e3779b1u;
            s = wangHash32(s);
            rng_state.b32 = s;
            return s;
        }

        double RandomGenerator::random() {
            uint32_t r = rand();
            return static_cast<double>(r) * (1.0 / 4294967296.0);
        }

        double RandomGenerator::random(double max) {
            return random() * max;
        }

        double RandomGenerator::random(double min, double max) {
            return random() * (max - min) + min;
        }

        double RandomGenerator::randomNormal(double stddev) {
            if (last_randomnormal != std::numeric_limits<double>::infinity())
            {
                double r = last_randomnormal;
                last_randomnormal = std::numeric_limits<double>::infinity();
                return r * stddev;
            }

            double u1 = random();
            double u2 = random();
            if (u1 <= 0.0)
                u1 = std::numeric_limits<double>::min();

            double radius = sqrt(-2.0 * log(u1));
            double theta = 2.0 * M_PI * u2;

            last_randomnormal = radius * cos(theta);
            return radius * sin(theta) * stddev;
        }

        void RandomGenerator::setState(const std::string &statestr) {
            if (statestr.size() < 3 || statestr.find("0x") != 0)
                return;

            uint32_t v = 0;
            std::stringstream ss(statestr);
            ss >> std::hex >> v;
            if (!ss.fail())
            {
                rng_state.b32 = v;
                last_randomnormal = std::numeric_limits<double>::infinity();
            }
        }

        std::string RandomGenerator::getState() const {
            std::stringstream ss;
            ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << rng_state.b32;
            return ss.str();
        }
    }
}
