#include <string>
#include <tuple>
#include <optional>

#include "system.hpp"

namespace love {
    namespace system {
        std::string getOS() {
            return "Wii";
        }

        int getProcessorCount() {
            return 1;
        }

        std::tuple<PowerState, std::optional<int>, std::optional<int>> getPowerInfo() {
            return std::make_tuple(PowerState::AC, std::nullopt, std::nullopt);
        }
    }
}