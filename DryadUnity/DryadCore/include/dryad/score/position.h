#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/note.h" // child type

namespace Dryad
{

class Position : public ScoreTraits<Position>
{

public:

    Position(Measure& parent)
        : ScoreTraits(parent)
    {
    }
};

}
