#pragma once

#include "dryad/result.h"
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
        , _lastUncommittedPositionCache(nullptr)
    {
    }

    U32 uncommittedPhraseCount()
    {
        U32 count = 0;

        for (auto phraseIterator = getChildren().rbegin(); phraseIterator != getChildren().rend(); phraseIterator++)
        {
            if(phraseIterator->isCommitted())
                break;

            count++;
        }

        return count;
    }

    Position* lastUncommittedPosition()
    {
        if(!_lastUncommittedPositionChanged)
            return _lastUncommittedPositionCache;

        Phrase* lastUncommittedPhrase = getLastUncommittedChild();
        if(lastUncommittedPhrase == nullptr)
            return nullptr;

        Measure* lastUncommittedMeasure = lastUncommittedPhrase->getLastUncommittedChild();
        if(lastUncommittedMeasure == nullptr)
            return nullptr;

        _lastUncommittedPositionCache = lastUncommittedMeasure->getLastUncommittedChild();
        return _lastUncommittedPositionCache;
    }

    Result<> commitPositionsUntil(ScoreTime commitUntilScoreTime)
    {
        Position* position = lastUncommittedPosition();

        while (position != nullptr || (position->getScoreTime() > commitUntilScoreTime))
        {
            position->commit();
            _lastUncommittedPositionChanged = true;
            position = position->next();
        }

        return Success;
    }



private:

    bool _lastUncommittedPositionChanged;
    Position* _lastUncommittedPositionCache;

};

} // namespace Dryad