#include "config.h"
#include "runtime.h"
#include "int.h"

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <cmath>
#include <sstream>

namespace love
{

static int w__gc(void*)
{
    return 0;
}

static int w__tostring(void*)
{
    return 0;
}

static int w__type(void*)
{
    return 0;
}

static int w__typeOf(void*)
{
    return 0;
}

static int w__eq(void*)
{
    return 0;
}

typedef uint64 ObjectKey;

}