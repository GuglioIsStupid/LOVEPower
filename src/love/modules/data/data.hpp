#pragma once

namespace {
    [[maybe_unused]]
    static std::vector<uint8_t> base64Decode(const std::string &in) {
        static constexpr char sDecodingTable[128] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 0, 64, 64,
            64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
            64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
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

    [[maybe_unused]]
    static std::vector<uint8_t> hexDecode(const std::string &in) {
        std::vector<uint8_t> out;
        for (size_t i = 0; i < in.size(); i += 2) {
            std::string byteStr = in.substr(i, 2);
            out.push_back(uint8_t(std::stoi(byteStr, nullptr, 16)));
        }
        return out;
    }
}

namespace love {
    namespace data {
        void __init(sol::state &luastate);
        void __registerTypes(sol::state &luastate);

        sol::object decode(const std::string &containerType,
                            const std::string &format,
                            const std::string &sourceData, 
                            sol::this_state ts);
    }
}