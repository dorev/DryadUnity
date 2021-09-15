#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Score;

struct Voice : MonitorCount<Voice>
{
    Voice()
        : octave(5)
        , name("")
    {}

    uint octave;
    String name;
    WeakPtr<Score> parentScore;
};

} // namespace Dryad