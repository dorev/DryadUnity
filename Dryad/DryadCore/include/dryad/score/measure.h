#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/position.h" // child type

namespace Dryad
{

class Measure : public ScoreTraits<Measure>
{

public:

    Measure(Phrase& parent)
        : ScoreTraits(parent)
    {
    }
};

}
