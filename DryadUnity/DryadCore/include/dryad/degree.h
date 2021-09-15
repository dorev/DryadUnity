#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Scale;

struct Degree : MonitorCount<Degree>
{
    Degree(uint intervalFromRoot, WeakPtr<Scale> parentScale, InitializerList<uint> chordIntervals = {})
        : intervalFromRoot(intervalFromRoot)
        , chordIntervals(chordIntervals)
        , parentScale(parentScale)
    {}

    uint intervalFromRoot;
    Vector<uint> chordIntervals;
    WeakPtr<Scale> parentScale;
    WeakPtr<Degree> next;
    WeakPtr<Degree> previous;
};

} // namespace Dryad