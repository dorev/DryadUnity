#pragma once

#include "dryad/types.h"
#include "dryad/configurations.h"
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

        void Commit()
        {
            _committed = true;
        }

        Instant* Next() const
        {
            return _next;
        }

        Instant* Prev() const
        {
            return _prev;
        }

    private:
        Scale* _scale;
        LandscapeNode* _landscapeNode;
        Instant* _next;
        Instant* _prev;
        bool _committed;
        ScoreTime _scoreTime;
        ScoreNote _notes[MAX_NOTES_PER_INSTANT];
    };
}
