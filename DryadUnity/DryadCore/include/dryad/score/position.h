#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/note.h" // child type

namespace Dryad
{

struct MotifLocalData
{
    MotifLocalData(U32 activeCount = 0, bool isMotifBeginning = false)
        : activeCount(activeCount)
        , isMotifBeginning(isMotifBeginning)
    {
    }

    U32 activeCount;
    bool isMotifBeginning;
};

class Position : public ScoreTraits<Position>
{

public:

    Position(Measure& parent)
        : ScoreTraits(parent)
        , _scoreTime(0)
        , _harmonyNode(nullptr)
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

    Map<U32, U32> getActiveMotifs() const
    {
        return _activeMotifs;
    }

    void addMotif(U32 motifId)
    {
        if(_activeMotifs.find(motifId) == _activeMotifs.end())
            _activeMotifs[motifId] = 1;
        else
            _activeMotifs[motifId]++;
    }

    Result<> removeMotif(U32 motifId)
    {
        if(_activeMotifs.find(motifId) == _activeMotifs.end())
            return {ErrorCode::MotifDoesNotExist};
        else if(_activeMotifs.at(motifId) == 0)
            return {ErrorCode::MotifAlreadyFullyDeactivated};
        else
            _activeMotifs[motifId]--;
    }

    Result<> addMotif(Motif* motif)
    {
        if(motif == nullptr)
            return {ErrorCode::NullPointer};

        const MotifDescriptor& descriptor = motif->getDescriptor();

        if(_motifsData.find(motif) == _motifsData.end())
            _motifsData[motif] = {1, true};
        else
            _activeMotifs[motif->getDescriptor().getId()]++;
    }

private:

    ScoreTime _scoreTime;
    Map<U32, U32> _activeMotifs;
    Map<Motif*, MotifLocalData> _motifsData;
    HarmonyNode* _harmonyNode;
};

}
