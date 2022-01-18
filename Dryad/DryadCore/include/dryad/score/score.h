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

    U32 UncommittedPhraseCount()
    {
        U32 count = 0;

        for (auto phraseIterator = GetChildren().rbegin(); phraseIterator != GetChildren().rend(); phraseIterator++)
        {
            if(phraseIterator->IsCommitted())
                break;

            count++;
        }

        return count;
    }

    Position* GetFirstUncommittedPosition()
    {
        if(!_lastUncommittedPositionChanged)
            return _lastUncommittedPositionCache;

        Phrase* phrase = GetLastUncommittedChild();
        if(phrase == nullptr)
            return nullptr;

        Measure* measure = phrase->GetLastUncommittedChild();
        if(measure == nullptr)
            return nullptr;

        _lastUncommittedPositionCache = measure->GetLastUncommittedChild();
        return _lastUncommittedPositionCache;
    }

    Result<> CommitPositionsUntil(ScoreTime commitUntilScoreTime)
    {
        Position* position = GetFirstUncommittedPosition();

        while (position != nullptr && (position->GetScoreTime() < commitUntilScoreTime))
        {
            position->Commit();
            _lastUncommittedPositionChanged = true;
            position = position->Next();
        }

        return Success;
    }

private:

    bool _lastUncommittedPositionChanged;
    Position* _lastUncommittedPositionCache;
};

} // namespace Dryad