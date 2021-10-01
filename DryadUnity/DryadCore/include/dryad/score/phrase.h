#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/measure.h" // child type

namespace Dryad
{

class Phrase : public ScoreTraits<Phrase>
{

public:

    Phrase(Score& parent)
        : ScoreTraits(parent)
    {
    }
};

}
