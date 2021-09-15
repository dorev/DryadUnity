#pragma once

#include "types.h"

namespace Dryad
{

struct Degree;

struct ScaleConfig
{
    ScaleConfig(uint root = 0, Accidental accidental = Accidental::Sharp)
        : root(root)
        , accidental(accidental)
    {}

    uint root;
    Accidental accidental;
};

} // namespace Dryad