#pragma once

#define DELETE_COPY_CONSTRUCTOR(Class) \
    Class(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class& operator=(Class&&) = delete;

#define UNUSED(variable) (void)variable;

#include <iostream>
#define DEBUG_BREAK(x) \
{ \
    std::cout << "\n\n" << x << "\n --> " << __FILE__ << " l." << __LINE__ << "\n\n"; \
    *(reinterpret_cast<unsigned int*>(0xBAAAAADBADBADBAD)) = 0U; \
    throw; \
}
