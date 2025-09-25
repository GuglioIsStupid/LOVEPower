#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "data.hpp"
#include <sol/sol.hpp>
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace data {
        sol::object decode(const std::string &containerType,
                            const std::string &format,
                            const std::string &sourceData, 
                            sol::this_state ts)
        {
            sol::state_view lua(ts);
            std::vector<uint8_t> decoded;

            if (format == "base64") {
                decoded = base64Decode(sourceData);
            } else if (format == "hex") {
                decoded = hexDecode(sourceData);
            } else {
                throw std::runtime_error("Invalid format: only 'hex' or 'base64' allowed");
            }

            if (containerType == "string") {
                return sol::make_object(lua, std::string(decoded.begin(), decoded.end()));
            } else if (containerType == "data") {
                return sol::make_object(lua, love::data::Data(std::move(decoded)));
            } else {
                throw std::runtime_error("Invalid container type: only 'string' or 'data' allowed");
            }
        }

        void __init(sol::state &luastate) {
            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::data::Data>(
                "Data",
                sol::no_constructor,
                "getBytes", [](love::data::Data &self) {
                    return std::string(self.bytes.begin(), self.bytes.end());
                },
                "getSize", [](love::data::Data &self) {
                    return self.bytes.size();
                }
            );
        }
    }
}

int luaopen_love_data(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["data"] = luastate.create_table_with(
        "decode", love::data::decode
    );

    return 1;
}