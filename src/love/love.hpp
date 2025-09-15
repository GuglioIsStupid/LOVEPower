#pragma once

#include <string>
#include <tuple>

#include <sol/sol.hpp>

namespace love {
    void __init(sol::state &luastate);
    
    inline std::string _console                            = "Wii";
    inline std::string _os                                 = "Wii";
    inline std::tuple<int, int, int, std::string> _version = std::make_tuple(0, 1, 0, "UNKNOWN");
    inline bool _deprecationOutput                         = false;

    bool hasDeprecationOutput();
    void setDeprecationOutput(bool deprecationOutput);
    std::tuple<int, int, int, std::string> getVersion();
}
