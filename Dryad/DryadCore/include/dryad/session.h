#pragma once

#include "dryad/error.h"
#include "dryad/idprovider.h"
#include "dryad/midinote.h"
#include "dryad/result.h"
#include "dryad/types.h"
#include "dryad/utils.h"

#include "dryad/harmony/composer.h"
#include "dryad/harmony/note.h"

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

    Vector<MidiNote>&& Commit(ScoreTime deltaScoreTime)
    {
        return std::move(_score.Commit(deltaScoreTime));
    }

    Result<> Generate(ScoreTime scoreTime)
    {
    }

    //
    // Forwarding to composer
    //

    Result<> RegisterMotif(const String& motifName, const Motif& motif)
    {
        return _composer.RegisterMotif(motifName, motif);
    }

    Result<> RegisterLandscapeGraph(const String& landscapeName, const LandscapeGraph& landscapeGraph)
    {
        return _composer.RegisterLandscape(landscapeName, landscapeGraph);
    }

    Result<> AddMotif(const String& motifName, S32 amount = 1)
    {
        return _composer.AddMotif(motifName, amount);
    }

    Result<> SetLandscape(const String& landscapeName)
    {
        return _composer.TransitionToLandscape(landscapeName);
    }

private:
    Score _score;
    Composer _composer;
    IdProvider _idProvider;
};

} // namespace Dryad