#pragma once

#include "dryad/result.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/phrase.h" // child type

namespace Dryad
{

class Score : public ScoreTraits<Score>
{

public:

    Score(Session& parent)
        : ScoreTraits(parent)
        , _lastUncommittedPositionChanged(false)
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

    Position* getFirstUncommittedPosition()
    {
        if(!_lastUncommittedPositionChanged)
            return _lastUncommittedPositionCache;

        Phrase* phrase = getLastUncommittedChild();
        if(phrase == nullptr)
            return nullptr;

        Measure* measure = phrase->getLastUncommittedChild();
        if(measure == nullptr)
            return nullptr;

        _lastUncommittedPositionCache = measure->getLastUncommittedChild();
        return _lastUncommittedPositionCache;
    }

    Result<> commitPositionsUntil(ScoreTime commitUntilScoreTime)
    {
        Position* position = getFirstUncommittedPosition();

        while (position != nullptr && (position->getScoreTime() < commitUntilScoreTime))
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