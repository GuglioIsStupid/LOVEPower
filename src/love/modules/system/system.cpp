#include <string>
#include <tuple>
#include <optional>
#include <sol/sol.hpp>

#include "system.hpp"

namespace love {
    namespace system {
        void __init(sol::state &luastate) {
            sol::table system = luastate.create_table_with(
                "getOS", getOS,
                "getProcessorCount", getProcessorCount,
                "getPowerInfo", getPowerInfo
            );

            luastate["love"]["system"] = system;
        }
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