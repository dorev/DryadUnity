#pragma once

#include "dryad/definitions.h"
#include "dryad/types.h"
#include <random>

namespace Dryad
{

namespace Random
{
    template <class T, class U>
    auto range(T min, U max) -> decltype(min + max)
    {
        if (min == max)
        {
            return min;
        }
        if (max < min)
        {
            std::swap(min, max);
        }

        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<decltype(min + max)> distribution(min, max);
        return distribution(generator);
    }

    template <class T>
    const auto& in(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }

    bool fiftyFifty();
};

template <class T>
bool contains(const T& value, const Vector<T>& vector)
{
    for (U64 i = 0; i < vector.size(); ++i)
    {
        if (value == vector[i])
        {
            return true;
        }
    }

    return false;
}


void getEquivalentDurationPairs(U64 duration, Vector<Pair<U64, U64>>& solutions);

bool isPowerOf2(U64 value);

} // namespace Dryad
