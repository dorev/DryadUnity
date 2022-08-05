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
        , _scaleRoot(0)
        , _landscapeNode(nullptr)
        , _next(nullptr)
        , _prev(nullptr)
        , _committed(false)
        , _scoreTime(0)
        , _notes()
    {
    }

    ScoreTime GetScoreTime() const
    {
        return _scoreTime;
    }

    void Commit(bool commitNote = true)
    {
        _committed = commitNote;
    }

    bool IsCommitted() const
    {
        return _committed;
    }

    bool IsNotCommitted() const
    {
        return !_committed;
    }

    Instant* GetNext() const
    {
        return _next;
    }

    Instant* GetPrev() const
    {
        return _prev;
    }

    void SetNext(Instant* instant)
    {
        _next = instant;
    }

    void SetPrev(Instant* instant)
    {
        _prev = instant;
    }

    // Deprecating, we have to find another way to explore the score without returning new objects!!
    /*
    void InsertMidiNotes(Vector<MidiNote>& destinationVector)
    {
        for (U32 i = 0; i < MAX_NOTES_PER_INSTANT; ++i)
        {
            const ScoreNote& note = _notes[i];
            if (note.GetWindowDuration() > 0)
            {
                destinationVector.emplace(
                    destinationVector.end(),
                    note.GetMidiValue(),
                    note.GetVoiceId(),
                    note.GetWindowDuration(),
                    GetScoreTime() 
                );
            }
        }
    }
    */

    Result<> AddNote(const ScoreNote& noteToAdd)
    {
        for (U32 i = 0; i < MAX_NOTES_PER_INSTANT; ++i)
        {
            ScoreNote& note = _notes[i];

            if (note.GetDuration() == 0)
            {
                note = noteToAdd;
                return Success;
            }
        }

        return { ErrorCode::UnableToStoreData };
    }

    const ScoreNote* GetNotes() const
    {
        return _notes;
    }

private:
    Scale* _scale;
    Note _scaleRoot;
    LandscapeNode* _landscapeNode;
    Instant* _next;
    Instant* _prev;
    bool _committed;
    ScoreTime _scoreTime;
    ScoreNote _notes[MAX_NOTES_PER_INSTANT];
};

}
