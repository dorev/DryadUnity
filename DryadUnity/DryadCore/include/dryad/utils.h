#pragma once

#include "dryad/definitions.h"
#include "dryad/types.h"
#include <random>
#include <utility>

namespace Dryad
{

namespace Random
{
    template <class T, class U, class R = decltype(std::declval<T>() + std::declval<U>())>
    auto range(T min, U max) -> R
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
        std::uniform_int_distribution<R> distribution(min, max);
        return distribution(generator);
    }

    template <class T>
    const auto& in(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }
};

template <class T>
bool contains(const Vector<T>& vector, const T& value)
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

template <class T, class U>
bool contains(const Map<T, U>& map, const T& value)
{
    return map.find(value) != map.end();
}


void getEquivalentDurationPairs(U64 duration, Vector<Pair<U64, U64>>& solutions);

bool isPowerOf2(U64 value);


/*
ScoreTime timeMsToScoreTime(TimeMs durationMs, U32 tempo)
{
    // Timestamp validity is already checked in the calling function
    TimeMs beatDuration = tempo / 60 * 1000;
    return durationMs / beatDuration * Constants::Duration::Quarter;
}
*/

} // namespace Dryad
