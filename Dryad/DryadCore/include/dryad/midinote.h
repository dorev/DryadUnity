#pragma once

#include "dryad/types.h"
namespace Dryad
{

class MidiNote
{
public:

    MidiNote() = delete;
    MidiNote(U8 value = 0, TimeMs duration = 0, TimestampMs startTime = 0)
        : value(value)
        , duration(duration)
        , startTime(startTime)
    {
    }

    U8 value;
    TimeMs duration;
    TimestampMs startTime;
};

} // namespace Dryad