#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/phrase.h" // child type

namespace Dryad
{

class Score : public ScoreTraits<Score>
{

public:

    Score(Session& parent)
        : ScoreTraits(parent)
    {
    }
};

} // namespace Dryad