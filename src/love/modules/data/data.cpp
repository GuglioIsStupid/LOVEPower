#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "data.hpp"

namespace {
    static std::vector<uint8_t> base64Decode(const std::string &in) {
        static constexpr char sDecodingTable[] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 0, 64, 64,
            64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
            64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
        };

        std::vector<uint8_t> out;
        int val = 0, valb = -8;
        for (unsigned char c : in) {
            if (std::isspace(c)) continue;
            if (c > 127 || sDecodingTable[c] == 64) break;
            val = (val << 6) + sDecodingTable[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(uint8_t((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    static std::vector<uint8_t> hexDecode(const std::string &in) {
        std::vector<uint8_t> out;
        if (in.size() % 2 != 0) throw std::runtime_error("Invalid hex string length");
        for (size_t i = 0; i < in.size(); i += 2) {
            uint8_t byte = (uint8_t) std::stoi(in.substr(i, 2), nullptr, 16);
            out.push_back(byte);
        }
        return out;
    }
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
                return sol::make_object(lua, decoded);
            } else {
                throw std::runtime_error("Invalid container type: only 'string' or 'data' allowed");
            }
        }
    }
}
