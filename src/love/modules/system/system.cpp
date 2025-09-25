#include <string>
#include <tuple>
#include <optional>

#include "system.hpp"
#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace system {
        std::string getOS() {
            return "Wii";
        }

        int getProcessorCount() {
            return 1;
        }

        std::tuple<std::string, std::optional<int>, std::optional<int>> getPowerInfo() {
            return std::make_tuple("AC", std::nullopt, std::nullopt);
        }
    }
}

int luaopen_love_system(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["system"] = luastate.create_table_with(
        "getOS", love::system::getOS,
        "getProcessorCount", love::system::getProcessorCount,
        "getPowerInfo", love::system::getPowerInfo
    );

    return 1;
}