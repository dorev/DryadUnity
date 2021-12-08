#pragma once

#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/types.h"

#include "dryad/composer/motifwriter.h"
#include "dryad/composer/progressionwriter.h"

#include "dryad/harmony/harmonygraph.h"
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

    Result<> generatePhrases(U32 amount)
    {
        if(amount == 0)
            return {ErrorCode::UselessCall};

        // extend using the current active harmony graph
        return Success;
    }

    Result<> generateNotesUntil(ScoreTime scoreTime)
    {
        Position* position = _score.getFirstUncommittedPosition();
        if(position == nullptr)
            return {ErrorCode::PositionDoesNotExist};

        if(scoreTime < position->getScoreTime())
            return {ErrorCode::CannotWritePastElements};

        // FIND A WAY TO QUERY WHERE A MOTIF STARTS

        if (!motifsChanged())
        {
            for (auto& motifChange : _motifsChanges)
            {
                const String& motifName = motifChange.first;
                U32 amount = motifChange.second;

                if (amount > 0)
                    _motifWriter.increaseMotifPresence(_motifs.at(motifName), amount);
                else if (amount < 0)
                    _motifWriter.decreaseMotifPresence(_motifs.at(motifName), amount);
            }
        }

        // extend all active motifs at least until scoreTime
        // 
        return Success;
    }

    void transitionToGraph(const String& graphName)
    {
        // LATER!!
    }

    Result<> removeMotif(const String& motifName)
    {
        if(!motifExists(motifName))
            return {ErrorCode::MotifDoesNotExist};

        if(_motifsActiveInstances[motifName] == 0)
            return {ErrorCode::MotifAlreadyFullyDeactivated};

        _motifsChanges[motifName]--;
        return Success;
    }

    Result<> addMotif(const String& motifName)
    {
         if(!motifExists(motifName))
            return {ErrorCode::MotifDoesNotExist};

        _motifsChanges[motifName]++;
        return Success;
    }

    Result<> harmonizeFrom(Position& position)
    {
        return _harmonizer.harmonizeFrom(position);
    }

    Result<> registerMotif(const String& motifName, const Motif& motif)
    {
        if (motifExists(motifName))
            return { ErrorCode::MotifAlreadyExists };

        _motifs[motifName] = motif;
        _motifsChanges[motifName] = 0;
        return Success;
    }

    Result<> registerHarmonyGraph(const String& graphName, const HarmonyGraph& graph)
    {
        if (graphExists(graphName))
            return { ErrorCode::GraphAlreadyExists };

        _graphs[graphName] = graph;
        return Success;
    }

private:

    bool motifsChanged() const
    {
        for (auto& motifChange : _motifsChanges)
        {
            U32 amount = motifChange.second;
            if(amount != 0)
                return true;
        }

        return false;
    }

    bool motifExists(const String& motifName) const
    {
        return _motifs.find(motifName) != _motifs.end();
    }

    bool motifExists(const MotifDescriptor& motifDescriptor) const
    {
        return _motifs.find(motifDescriptor.getName()) != _motifs.end();
    }

    bool graphExists(const String& graphName) const
    {
        return _graphs.find(graphName) != _graphs.end();
    }

    bool graphExists(const HarmonyGraphDescriptor& graphDescriptor) const
    {
        return _graphs.find(graphDescriptor.getName()) != _graphs.end();
    }

    Harmonizer _harmonizer;
    MotifWriter _motifWriter;
    ProgressionWriter _progressionWriter;

    Map<String, Motif> _motifs;
    Map<String, U32> _motifsChanges;
    Map<String, U32> _motifsActiveInstances;

    Map<String, HarmonyGraph> _graphs;
    String _currentGraph;

    Score& _score;
};

} // namespace Dryad