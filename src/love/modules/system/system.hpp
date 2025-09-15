#pragma once
#include <string>

namespace love {
    namespace system {
        std::string   getOS();
        int           getProcessorCount();
        
        std::tuple<std::string, std::optional<int>, std::optional<int>> getPowerInfo();
    }
}