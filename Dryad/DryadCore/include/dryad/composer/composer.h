#pragma once

#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/types.h"

#include "dryad/composer/motifwriter.h"
#include "dryad/composer/progressionwriter.h"

#include "dryad/harmony/landscapegraph.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/harmonizer.h"

#include "dryad/score/score.h"

namespace Dryad
{

class Composer
{

public:

    Composer(Score& score)
        : _score(score)
        , _harmonizer(_score)
        , _motifWriter(_score)
        , _progressionWriter(_score)
    {
    }

    Result<> GeneratePhrases(U32 amount)
    {
        if(amount == 0)
            return {ErrorCode::UselessCall};

        // extend using the current active harmony graph
        return Success;
    }

    Result<> GenerateNotesUntil(ScoreTime scoreTime)
    {
        Position* position = _score.GetFirstUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        if(scoreTime < position->GetScoreTime())
            return {ErrorCode::CannotWritePastElements};

        // FIND A WAY TO QUERY WHERE A MOTIF STARTS

        if (!MotifsChanged())
        {
            for (auto& motifChange : _motifsChanges)
            {
                const String& motifName = motifChange.first;
                U32 amount = motifChange.second;

                if (amount > 0)
                    _motifWriter.IncreaseMotifPresence(_motifs.at(motifName), amount);
                else if (amount < 0)
                    _motifWriter.DecreaseMotifPresence(_motifs.at(motifName), amount);
            }
        }

        // extend all active motifs at least until scoreTime
        // 
        return Success;
    }

    void TransitionToGraph(const String& graphName)
    {
        // LATER!!
    }

    Result<> RemoveMotif(const String& motifName)
    {
        if(!MotifExists(motifName))
            return {ErrorCode::MotifDoesNotExist};

        if(_motifsActiveInstances[motifName] == 0)
            return {ErrorCode::MotifAlreadyFullyDeactivated};

        _motifsChanges[motifName]--;
        return Success;
    }

    Result<> AddMotif(const String& motifName)
    {
         if(!MotifExists(motifName))
            return {ErrorCode::MotifDoesNotExist};

        _motifsChanges[motifName]++;
        return Success;
    }

    Result<> HarmonizeFrom(Position& position)
    {
        return _harmonizer.HarmonizeFrom(position);
    }

    Result<> RegisterMotif(const String& motifName, const Motif& motif)
    {
        if (MotifExists(motifName))
            return { ErrorCode::MotifAlreadyExists };

        _motifs[motifName] = motif;
        _motifsChanges[motifName] = 0;
        return Success;
    }

    Result<> RegisterLandscapeGraph(const String& graphName, const LandscapeGraph& graph)
    {
        if (GraphExists(graphName))
            return { ErrorCode::GraphAlreadyExists };

        _landscapes[graphName] = graph;
        return Success;
    }

private:

    bool MotifsChanged() const
    {
        for (auto& motifChange : _motifsChanges)
        {
            U32 amount = motifChange.second;
            if(amount != 0)
                return true;
        }

        return false;
    }

    bool MotifExists(const String& motifName) const
    {
        return _motifs.find(motifName) != _motifs.end();
    }

    bool MotifExists(const MotifDescriptor& motifDescriptor) const
    {
        return _motifs.find(motifDescriptor.GetName()) != _motifs.end();
    }

    bool GraphExists(const String& graphName) const
    {
        return _landscapes.find(graphName) != _landscapes.end();
    }

    bool GraphExists(const LandscapeGraphDescriptor& graphDescriptor) const
    {
        return _landscapes.find(graphDescriptor.GetName()) != _landscapes.end();
    }

    Harmonizer _harmonizer;
    MotifWriter _motifWriter;
    ProgressionWriter _progressionWriter;

    Map<String, Motif> _motifs;
    Map<String, U32> _motifsChanges;
    Map<String, U32> _motifsActiveInstances;

    Map<String, LandscapeGraph> _landscapes;
    U32 _currentLandscape;

    Score& _score;
};

} // namespace Dryad