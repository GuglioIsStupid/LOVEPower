#include <string>
#include <tuple>

#include <sol/sol.hpp>

#include "love.hpp"

namespace love {
    void __init(sol::state &luastate) {
        luastate["love"] = luastate.create_table_with(
            "_console", love::_console,
            "_os", love::_os,
            "hasDeprecationOutput", love::hasDeprecationOutput,
            "setDeprecationOutput", love::setDeprecationOutput,
            "getVersion", love::getVersion
        );
    }

    bool hasDeprecationOutput() {
        return _deprecationOutput;
    }

    void setDeprecationOutput(bool deprecationOutput) {
        _deprecationOutput = deprecationOutput;
    }

    std::tuple<int, int, int, std::string> getVersion() {
        return _version;
    }
}
