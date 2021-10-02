#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/phrase.h" // child type

namespace Dryad
{

using Frame = Vector<Pair<Note, TimestampMs>>;

class Score : public ScoreTraits<Score>
{

public:

    Score(Session& parent)
        : ScoreTraits(parent)
    {
    }


    U32 uncommittedPhraseCount()
    {
        return 0;
    }

    Position* lastUncommittedPosition()
    {
        Phrase* lastUncommittedPhrase = getLastUncommittedChild();

        if(lastUncommittedPhrase == nullptr)
            return nullptr;

        Measure* lastUncommittedMeasure = lastUncommittedPhrase->getLastUncommittedChild();

        if(lastUncommittedMeasure == nullptr)
            return nullptr;

        return lastUncommittedMeasure->getLastUncommittedChild();
    }

    Frame commitPositionsUntil(TimestampMs commitTimestamp)
    {
        return {};
    }

};

} // namespace Dryad