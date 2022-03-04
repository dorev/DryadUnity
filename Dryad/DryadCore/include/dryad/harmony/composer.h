#pragma once

#include "dryad/error.h"
#include "dryad/result.h"
#include "dryad/types.h"

#include "dryad/harmony/landscapegraph.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/harmonizer.h"


namespace Dryad
{

class Composer
{
public:
    Composer(Score& score)
        : _score(score)
        , _harmonizer(_score)
    {
    }

    Result<> GenerateNotesUntil(ScoreTime scoreTime)
    {
        Instant* instant = _score.GetFirstUncommittedInstant();
        if(instant == nullptr)
            return {ErrorCode::PositionDoesNotExist};
        if(scoreTime < instant->GetScoreTime())
            return {ErrorCode::CannotWritePastElements};

        //
        // FIND A WAY TO QUERY WHERE A MOTIF STARTS
        //

        if (!MotifsChanged())
        {
            for (auto& motifChange : _motifsChanges)
            {
                const String& motifName = motifChange.first;
                U32 amount = motifChange.second;
                if (amount != 0)
                    _motifsActiveInstances.at(motifName) += amount;
            }
        }

        // extend all active motifs at least until scoreTime
        // 
        return Success;
    }

    Result<> TransitionToLandscape(const String& landscapeName)
    {
        if (LandscapeExists(landscapeName))
            return { ErrorCode::LandscapeDoesNotExist };
        _previousLandscape = _currentLandscape;
        _currentLandscape = landscapeName;
        return Success;
    }

    Result<> AddMotif(const String& motifName, S32 amount)
    {
        if(!MotifExists(motifName))
            return { ErrorCode::MotifDoesNotExist };
        if (amount != 0)
        {
            if ((static_cast<S32>(_motifsChanges[motifName]) + amount) < 0)
                _motifsChanges[motifName] = 0;
            else
                _motifsChanges[motifName] += amount;
        }
        return Success;
    }

    Result<> HarmonizeFrom(Instant* instant)
    {
        if(instant != nullptr)
            return _harmonizer.HarmonizeFrom(*instant);
        return { ErrorCode::NullPointer };
    }

    Result<> RegisterMotif(const String& motifName, const Motif& motif)
    {
        if (MotifExists(motifName))
            return { ErrorCode::MotifAlreadyExists };
        _motifs[motifName] = motif;
        _motifsChanges[motifName] = 0;
        return Success;
    }

    Result<> RegisterLandscape(const String& landscapeName, const LandscapeGraph& landscapeGraph)
    {
        if (LandscapeExists(landscapeName))
            return { ErrorCode::LandscapeAlreadyExists };
        _landscapes[landscapeName] = landscapeGraph;
        return Success;
    }

private:

    bool LandscapeChanged() const
    {

    }

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

    bool LandscapeExists(const String& graphName) const
    {
        return _landscapes.find(graphName) != _landscapes.end();
    }

    bool LandscapeExists(const LandscapeGraphDescriptor& graphDescriptor) const
    {
        return _landscapes.find(graphDescriptor.GetName()) != _landscapes.end();
    }


    // COMMENT ON VA GÉRER LES CHANGEMENTS DE GAMME??

    Score& _score;
    Harmonizer _harmonizer;
    String _currentLandscape;
    String _previousLandscape;
    bool _landscapeChanged;
    Scale* _currentScale;
    Note _currentRoot;
    Map<String, Motif> _motifs;
    Map<String, U32> _motifsChanges;
    Map<String, U32> _motifsActiveInstances;
    Map<String, LandscapeGraph> _landscapes;

};

} // namespace Dryad