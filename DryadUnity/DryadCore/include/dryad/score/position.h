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
        , _scoreTime(0)
    {
    }

    void setScoreTime(ScoreTime scoreTime)
    {
        _scoreTime = scoreTime;
    }

    ScoreTime getScoreTime() const
    {
        return _scoreTime;
    }

private:

    ScoreTime _scoreTime;
};

}
