#pragma once

#include "dryad/types.h"
#include "dryad/configurations.h"
#include "dryad/score/scorenote.h"

namespace Dryad
{
    class Instant
    {
    public:
        Instant()
            : _scale(nullptr)
            , _landscapeNode(nullptr)
            , _next(nullptr)
            , _prev(nullptr)
            , _scoreTime(0)
            , _notes()
        {
        }

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

        Result<> AddNote(const ScoreNote& note)
        {

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
