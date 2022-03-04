#pragma once

#include "dryad/types.h"
#include "dryad/score/scorenote.h"

namespace Dryad
{
    class Instant
    {
    public:

        ScoreTime GetScoreTime() const
        {
            return _scoreTime;
        }

    private:
        ScoreTime _scoreTime;
        Scale* _scale;
        LandscapeNode* _landscapeNode;
        Vector<ScoreNote> _notes;
        Instant* _next;
        Instant* _prev;
    };
}
