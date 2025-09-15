#pragma once
#include <string>
#include <sol/sol.hpp>

namespace love {
    namespace system {
        void __init(sol::state &luastate);
        
        std::string   getOS();
        int           getProcessorCount();
        
        std::tuple<std::string, std::optional<int>, std::optional<int>> getPowerInfo();
    }
}