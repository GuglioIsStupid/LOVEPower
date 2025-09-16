#pragma once

namespace {
    static std::vector<uint8_t> base64Decode(const std::string &in);
    static std::vector<uint8_t> hexDecode(const std::string &in);
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