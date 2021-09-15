#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Position;
struct Voice;
struct Motif;

struct Note : MonitorCount<Note>
{
    Note(uint offset = 0)
        : offset(offset)
        , duration(0)
        , midi(-1)
        , accidental(Accidental::None)
        , octave(-1)
        , step(nullptr)
        , name(nullptr)
    {}

    Note(const Note& other)
        : offset(other.offset)
        , duration(other.duration)
        , midi(other.midi)
        , accidental(other.accidental)
        , octave(other.octave)
        , step(other.step)
        , name(other.name)
        , voice(other.voice)
        , motif(other.motif)
        , parentPosition(parentPosition)
    {}

    sint offset;
    uint duration;
    uint midi;
    Accidental accidental;
    uint octave;
    const char* step;
    const char* name;
    SharedPtr<Voice> voice;
    WeakPtr<MotifVariation> motif;
    WeakPtr<Position> parentPosition;
};

} // namespace Dryad