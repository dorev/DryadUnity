#pragma once

#include "dryadfacade.h"
#include <dryad/session.h>

class Facade::Internal
{
public:
    Internal();
    ~Internal();
    void RegisterMotif(const Motif& motif);
    void RegisterLandscape(const LandscapeGraph& landscape);
    void AddMotif(const unsigned motifId);
    void RemoveMotif(const unsigned motifId);
    void SetLandscape(const unsigned landscapeId);
    std::vector<NoteToPlay> Generate(const unsigned durationToGenerate);
    std::vector<NoteToPlay> Commit(unsigned durationToPlay);

private:
    Dryad::Session _session;
};
