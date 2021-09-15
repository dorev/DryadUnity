#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Note;
struct Motif;

struct MotifVariation : MonitorCount<MotifVariation>
{
    WeakPtr<Motif> parentMotif;
    Vector<SharedPtr<Note>> notes;
};

} // namespace Dryad