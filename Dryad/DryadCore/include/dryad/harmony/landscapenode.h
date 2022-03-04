#pragma once

#include "dryad/types.h"

namespace Dryad
{

class LandscapeNode
{
    U32 _duration;
    U32 _id;
    U32 _degree;
    S32 _inversion;
    S32 _accidental;
    S32 _shift;
    bool _entry;
    TriadVoicing _triadVoicing;
    ChordExtension _chordExtension;
};

} // namespace Dryad