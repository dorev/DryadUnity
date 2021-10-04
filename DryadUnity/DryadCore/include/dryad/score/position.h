#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/note.h" // child type

namespace Dryad
{

class Position : public ScoreTraits<Position>
{

public:

    Position(Measure& parent)
        : ScoreTraits(parent)
        , _scoreTime(0)
    {
    }

    void setScoreTime(ScoreTime scoreTime)
    {
        _scoreTime = scoreTime;
    }

    ScoreTime getScoreTime() const
    {
        return _scoreTime;
    }

    Map<String, U32> getActiveMotifs() const
    {
        return _activeMotifs;
    }

    void addMotif(const String& motifName)
    {
        if(_activeMotifs.find(motifName) == _activeMotifs.end())
            _activeMotifs[motifName] = 1;
        else
            _activeMotifs[motifName]++;
    }

    Result<> removeMotif(const String& motifName)
    {
        if(_activeMotifs.find(motifName) == _activeMotifs.end())
            return {ErrorCode::MotifDoesNotExist};
        else if(_activeMotifs.at(motifName) == 0)
            return {ErrorCode::MotifAlreadyFullyDeactivated};
        else
            _activeMotifs[motifName]--;
    }

private:

    ScoreTime _scoreTime;
    Map<String, U32> _activeMotifs;
};

}
