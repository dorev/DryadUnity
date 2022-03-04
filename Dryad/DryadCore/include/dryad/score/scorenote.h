#pragma once

#include "dryad/types.h"
#include "dryad/harmony/motif.h"

namespace Dryad
{
    class ScoreNote
    {
        U32 _midi;
        U32 _duration;
        Motif* _motif;
    };
}
