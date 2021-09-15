#include "constants.h"
#include "definitions.h"
#include "utils.h"

namespace Dryad
{

bool Random::fiftyFifty()
{
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<uint> distribution(0, 1);
    return distribution(generator) > 0;
}

void getEquivalentDurationPairs(uint duration, Vector<Pair<uint, uint>>& solutions)
{
    solutions.clear();

    const auto& fractions = Constants::allowedRythmicFractions;

    // Inverted solutions duplicates are deliberatly preserved
    for (uint i = 0; i < fractions.size(); ++i)
    {
        for (uint j = 0; j < fractions.size(); ++j)
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

bool isPowerOf2(uint value)
{
    uint setBits = 0;

    for (uint bit = 0; bit < (sizeof(uint) * 8); ++bit)
    {
        if (value & (1ULL << bit) && setBits++)
        {
            return false;
        }
    }

    return true;
}


} // namespace Dryad
