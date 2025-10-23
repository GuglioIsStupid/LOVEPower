// A very skimmed down version of LOVE's runtime.h

#ifndef LOVE_RUNTIME_H
#define LOVE_RUNTIME_H

#include <exception>
#include <algorithm>

namespace love
{
enum Registry
{
    REGISTRY_MODULES,
    REGISTRY_OBJECTS
};

}

#endif // LOVE_RUNTIME_H