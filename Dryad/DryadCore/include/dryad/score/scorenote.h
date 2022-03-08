#pragma once

#include "dryad/types.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/note.h"

namespace Dryad
{
    class ScoreNote
    {
        U32 _midi;
        U32 _duration;
        Note _scaleRoot;
        Motif* _motif;
        Voice* _voice;
    };
}
