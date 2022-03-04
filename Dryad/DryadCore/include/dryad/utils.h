#pragma once

#include "dryad/definitions.h"
#include "dryad/types.h"
#include "dryad/result.h"
#include "dryad/constants.h"
#include <random>
#include <utility>

namespace Dryad
{

namespace Random
{
    template <class T, class U, class R = decltype(std::declval<T>() + std::declval<U>())>
    auto Range(T min, U max) -> R
    {
        if (min == max)
            return min;
        if (max < min)
            std::swap(min, max);
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<R> distribution(min, max);
        return distribution(generator);
    }

    template <class T>
    const auto& From(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }
};

template <class T>
bool Contains(const Vector<T>& vector, const T& value)
{
    for (U64 i = 0; i < vector.size(); ++i)
    {
        if (value == vector[i])
            return true;
    }
    return false;
}

template <class T, class U>
bool Contains(const Map<T, U>& map, const T& value)
{
    return map.find(value) != map.end();
}

Result<> GetEquivalentDurationPairs(U64 duration, Vector<Pair<U64, U64>>& solutions)
{
    solutions.clear();
    const auto& fractions = Constants::AllowedRythmicFractions;

    // Inverted solutions duplicates are deliberatly preserved
    for (U64 i = 0; i < fractions.size(); ++i)
    {
        for (U64 j = 0; j < fractions.size(); ++j)
        {
            if (fractions[i] + fractions[j] == duration)
                solutions.emplace_back(fractions[i], fractions[j]);
        }
    }
    if (solutions.empty())
        return { ErrorCode::NoAvailableEquivalence };
    return Success;
}

bool IsPowerOf2(U64 value)
{
    U64 setBits = 0;
    for (U64 bit = 0; bit < (sizeof(U64) * 8); ++bit)
    {
        if (value & (1ULL << bit) && setBits++)
            return false;
    }
    return true;
}

} // namespace Dryad
