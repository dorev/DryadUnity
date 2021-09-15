#pragma once

#include "types.h"

namespace Dryad
{

struct Motif;
struct Phrase;
struct Voice;

struct Score
{
    Vector<SharedPtr<Motif>> motifs;
    Vector<SharedPtr<Voice>> voices;
    Vector<SharedPtr<Phrase>> phrases;
    SharedPtr<HarmonyGraph> graph;
};

} // namespace Dryad