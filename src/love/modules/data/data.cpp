#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "data.hpp"

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
                return sol::make_object(lua, decoded);
            } else {
                throw std::runtime_error("Invalid container type: only 'string' or 'data' allowed");
            }
        }
    }
}
