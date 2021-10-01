#pragma once

#include "dryad/score/traits/scoretraits.h"
#include "dryad/descriptors.h"

namespace Dryad
{

class Note
{
public:

    Note(S64 offset = 0)
        : committed(false)
        , offset(offset)
        , duration(0)
        , midi(0)
        , accidental(Accidental::None)
    {}

    bool committed;
    S64 offset;
    ScoreTime duration;
    U8 midi;
    Accidental accidental;

    VoiceDescriptor parentVoice;
    MotifDescriptor parentMotif;
};

} // namespace Dryad