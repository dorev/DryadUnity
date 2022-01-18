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
    void Generate(const unsigned durationToGenerate);
    std::vector<NoteToPlay> Play(unsigned durationToPlay);

private:
    Dryad::Session _session;
};
