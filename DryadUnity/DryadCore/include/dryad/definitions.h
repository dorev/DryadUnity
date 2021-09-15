#pragma once

#include "includes.h"

#define DEBUG_BREAK(x) \
{ \
    std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; \
    *(reinterpret_cast<unsigned int*>(0xBAAAAADBADBADBAD)) = 0U; \
    throw; \
}
