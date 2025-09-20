#pragma once

#include "mii.h"

namespace love {
    namespace mii {
        struct MiiSafe : public Mii {
            MiiSafe() = default;
            MiiSafe(const Mii& m) {
                *this = m;
            }
            std::string nameStr() const { return std::string(name); }
            std::string creatorStr() const { return std::string(creator); }
        };
    }
}