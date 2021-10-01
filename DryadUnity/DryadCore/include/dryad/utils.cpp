#include "dryad/constants.h"
#include "dryad/utils.h"

namespace Dryad
{

bool Random::fiftyFifty()
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<U64> distribution(0, 1);
    return distribution(generator) > 0;
}

void getEquivalentDurationPairs(U64 duration, Vector<Pair<U64, U64>>& solutions)
{
    solutions.clear();

    const auto& fractions = Constants::allowedRythmicFractions;

    // Inverted solutions duplicates are deliberatly preserved
    for (U64 i = 0; i < fractions.size(); ++i)
    {
        for (U64 j = 0; j < fractions.size(); ++j)
        {
            if (fractions[i] + fractions[j] == duration)
            {
                solutions.emplace_back(fractions[i], fractions[j]);
            }
        }
    }

    if (solutions.empty())
    {
        DEBUG_BREAK("unable to split requested duration");
    }
}

bool isPowerOf2(U64 value)
{
    U64 setBits = 0;

    for (U64 bit = 0; bit < (sizeof(U64) * 8); ++bit)
    {
        if (value & (1ULL << bit) && setBits++)
        {
            return false;
        }
    }

    return true;
}


} // namespace Dryad
