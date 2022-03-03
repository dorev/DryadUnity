#pragma once

#include "dryad/error.h"
#include "dryad/idprovider.h"
#include "dryad/midinote.h"
#include "dryad/result.h"
#include "dryad/types.h"
#include "dryad/utils.h"

#include "dryad/composer/composer.h"

#include "dryad/score/score.h"



/*
en bout de ligne on a jamais besoin de tempo ici, on indiquement seulement combien
de mesures ou de beat on veut générer/commit

le tempo va être géré par le player dans tout ça, pas pas Dryad qui est le "générateur"
de la musique
*/


namespace Dryad
{

class Session
{

public:

    Session()
        : _committedScoreTime(0)
        , _generatedScoreTime(0)
        , _score(*this)
        , _composer(_score)
    {
    }

    Score& GetScore()
    {
        return _score;
    }

    Session& GetSession()
    {
        return *this;
    }

    Result<Vector<MidiNote>> Commit(ScoreTime deltaScoreTime)
    {
        // Early out
        if (deltaScoreTime == 0)
            return { ErrorCode::UselessCall };
        Position* position = _score.GetFirstUncommittedPosition();
        if (position == nullptr)
            return { ErrorCode::NothingToCommit };

        // Gather newly committed notes
        _committedScoreTime += deltaScoreTime;
        Vector<MidiNote> result;
        while (position != nullptr && position->GetScoreTime() < _committedScoreTime)
        {
            for (const Note& note : position->GetChildren())
                result.emplace_back(note.GetMidi(), note.GetDuration(), position->GetScoreTime());
            position->Commit();
            position = position->Next();
        }
        return result;
    }

    Result<> Generate(ScoreTime scoreTime)
    {
        // check if any motif changed, else bailout
        // regenerate un-committed motifs cells that has not been started yet
    }

    // Forwarding to composer

    Result<> RegisterMotif(const String& motifName, const Motif& motif)
    {
        return _composer.RegisterMotif(motifName, motif);
    }

    Result<> RegisterLandscapeGraph(const String& landscapeName, const LandscapeGraph& landscapeGraph)
    {
        return _composer.RegisterLandscape(landscapeName, landscapeGraph);
    }

    Result<> RemoveMotif(const String& motifName)
    {
        return _composer.RemoveMotif(motifName);
    }

    Result<> AddMotif(const String& motifName)
    {
        return _composer.AddMotif(motifName);
    }

    Result<> SetLandscape(const String& landscapeName)
    {
        return _composer.TransitionToLandscape(landscapeName);
    }

private:
    
    IdProvider _idProvider;
    ScoreTime _committedScoreTime;
    ScoreTime _generatedScoreTime;

    Score _score;
    Composer _composer;
    Map<String, Motif> _motifs;
    Map<String, Voice> _voices;
    Map<String, LandscapeGraph> _landscapes;

    //void stop();
    //void setPhraseLength(uint phraseLength);
    //Error transitionToGraph(const String& name);
};

} // namespace Dryad