#include <string>
#include <tuple>

#include <sol/sol.hpp>

#include "love.hpp"

namespace love {
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
