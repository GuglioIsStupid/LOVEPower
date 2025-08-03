#pragma once
#include <string>

namespace {
    enum class PowerState {
        AC,
    };
}

namespace love {
    namespace system {
        std::string   getOS();
        int           getProcessorCount();
        
        std::tuple<PowerState, std::optional<int>, std::optional<int>> getPowerInfo();
    }
}