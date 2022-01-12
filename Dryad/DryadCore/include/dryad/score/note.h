#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/descriptors.h"
#include "dryad/constants.h"

namespace Dryad
{

class Note
{
public:

    Note(S64 harmonyOffset = 0)
        : harmonyOffset(harmonyOffset)
        , duration(Constants::Duration::Quarter)
        , midi(0)
    {}

    S64 harmonyOffset;
    ScoreTime duration;
    U8 midi;

    MotifDescriptor parentMotif;
};

} // namespace Dryad