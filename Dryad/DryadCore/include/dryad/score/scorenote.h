#pragma once

#include "dryad/types.h"
#include "dryad/harmony/motif.h"
#include "dryad/harmony/note.h"

namespace Dryad
{
class ScoreNote
{
public:
    ScoreNote()
        : _midi(0)
        , _duration(0)
        , _scaleRoot(0)
        , _motifInstanceId(0)
        , _motif(nullptr)
        , _voice(nullptr)
    {
    }

private:
    U32 _midi;
    U32 _duration;
    Note _scaleRoot;
    U32 _motifInstanceId;
    Motif* _motif;
    Voice* _voice;
};

}
