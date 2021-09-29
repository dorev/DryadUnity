#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/descriptors.h"

namespace Dryad
{

class Note
{
public:

    Note(S64 offset = 0)
        : offset(offset)
        , duration(0)
        , midi(0)
        , accidental(Accidental::None)
        , octave(0)
        , name(nullptr)
    {}

    S64 offset;
    ScoreTime duration;
    U8 midi;
    U8 octave;
    Accidental accidental;
    const char* name;
    VoiceDescriptor parentVoice;
    MotifDescriptor parentMotif;
};

} // namespace Dryad