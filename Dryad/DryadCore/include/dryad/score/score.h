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
        return std::move(Vector<MidiNote>());
    }

private:

    List<Instant> _instants;
    bool _changed;
    Instant* _firstUncommittedInstant;
    Instant* _cachedFirstUncommittedInstant;
};

} // namespace Dryad