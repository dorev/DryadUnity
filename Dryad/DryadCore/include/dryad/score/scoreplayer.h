#pragma once

#include "dryad/types.h"
#include "dryad/score/instant.h"
#include "dryad/score/score.h"

namespace Dryad
{

class ScorePlayer
{
public:
    ONLY_EXPLICIT_CONSTRUCTOR(ScorePlayer);

    ScorePlayer(Score& score)
        : _score(score)
        , _currentInstant(_score.GetFirstInstant())
        , _instantNoteIndex(0)
    {
    }

    ScoreTime GetCurrentTime() const
    {
        return _currentInstant->GetScoreTime();
    }

    const Instant* GetCurrentInstant() const
    {
        return _currentInstant;
    }

    const ScoreNote* GetCurrentNote() const
    {
        const Instant* instant = GetCurrentInstant();
        if (instant != nullptr)
            return &instant->GetNotes()[_instantNoteIndex];
        return nullptr;
    }

    const Instant* SeekFirstUncommittedInstant()
    {
        Instant* instant = _score.GetFirstUncommittedInstant();
        return UpdateCursor(instant);
    }

    const Instant* Rewind()
    {
        return UpdateCursor(_score.GetFirstInstant());
    }

    const ScoreNote* ReadNext()
    {
        // Scan instant notes to find the next non-zero-duration note
        while (TrySeekNext())
        {
            if (GetCurrentNote()->GetDuration() > 0)
                return GetCurrentNote();
        }
        return nullptr;
    }

    const ScoreNote* ReadNextCommitted()
    {
        // Scan instant notes to find the next non-zero-duration note
        while (TrySeekNext(true))
        {
            if (GetCurrentNote()->GetDuration() > 0)
                return GetCurrentNote();
        }
        return nullptr;
    }

private:
    bool TrySeekNext(bool instantMustBeCommitted = false)
    {
        if (_instantNoteIndex == (MAX_NOTES_PER_INSTANT - 1))
        {
            Instant* nextInstant = _currentInstant->GetNext();
            if (nextInstant == nullptr || (instantMustBeCommitted && nextInstant->IsNotCommitted()))
                return false;
            UpdateCursor(nextInstant, 0);
        }
        else
            UpdateCursor(_currentInstant, ++_instantNoteIndex);
        return true;
    }

    const Instant* UpdateCursor(Instant* instant, U32 noteIndex = 0)
    {
        _instantNoteIndex = noteIndex;
        _currentInstant = instant;
        return _currentInstant;
    }

    Score& _score;
    Instant* _currentInstant;
    U32 _instantNoteIndex;
};




} // namespace Dryad