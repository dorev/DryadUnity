#pragma once

#include "dryad/error.h"
#include "dryad/idprovider.h"
#include "dryad/midinote.h"
#include "dryad/result.h"
#include "dryad/types.h"
#include "dryad/utils.h"

#include "dryad/harmony/harmonycontext.h"
#include "dryad/harmony/note.h"

#include "dryad/score/score.h"

namespace Dryad
{

class Session
{
public:

    Session()
        : _harmonyContext()
        //, _scoreWriter(_score)
    {
    }

    Result<> Commit(ScoreTime deltaScoreTime)
    {
        return _score.Commit(deltaScoreTime);
    }

    //
    // Forwarding to Composer to write additionnal music
    //

    Result<> Generate(ScoreTime scoreTime)
    {
        return Success;
    }

    //
    // Forwarding to HarmonyContext to store musical environment data
    //

    Result<Motif*> CreateMotif(const String& motifName)
    {
        return _harmonyContext.RegisterMotif(_idProvider.CreateMotifDescriptor(motifName));
    }

    Result<LandscapeGraph*> CreateLandscape(const String& landscapeName)
    {
        return _harmonyContext.RegisterLandscape(_idProvider.CreateLandscapeGraphDescriptor(landscapeName));
    }

    Result<> SetMotifCount(const String& motifName, S32 amount = 1)
    {
        return _harmonyContext.SetMotifCount(motifName, amount);
    }

    Result<> IncrementMotifCount(const String& motifName, S32 increment = 1)
    {
        return _harmonyContext.IncrementMotifCount(motifName, increment);
    }

    Result<> DecrementMotifCount(const String& motifName, S32 decrement = 1)
    {
        return _harmonyContext.IncrementMotifCount(motifName, -1 * decrement);
    }

    Result<> SetLandscape(const String& landscapeName)
    {
        return _harmonyContext.SetLandscape(landscapeName);
    }

    const Score& GetScore() const
    {
        return _score;
    }

private:
    Score _score;
    HarmonyContext _harmonyContext;
    IdProvider _idProvider;
};

} // namespace Dryad
