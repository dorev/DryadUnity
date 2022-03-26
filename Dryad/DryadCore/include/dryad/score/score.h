#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"
#include "dryad/score/instant.h"

namespace Dryad
{

class Score
{
public:

    Instant* GetFirstUncommittedInstant()
    {

    }

    Vector<MidiNote>&& Commit(ScoreTime deltaScoreTime)
    {
        if (_firstUncommittedInstant != nullptr && deltaScoreTime > 0)
        {
            ScoreTime startTime = _firstUncommittedInstant->GetScoreTime();
            ScoreTime commitUntil = startTime + deltaScoreTime;
            Instant* instant = _firstUncommittedInstant;
            do
            {
                instant->Commit();
                instant = instant->Next();
            } while (instant->GetScoreTime() < commitUntil);
        }

        // FIXME
        return std::move(Vector<MidiNote>());
    }

private:

    List<Instant> _instants;
    bool _changed;
    Instant* _firstUncommittedInstant;
    Instant* _cachedFirstUncommittedInstant;
};

} // namespace Dryad