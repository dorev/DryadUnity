#pragma once

#include "dryad/types.h"
namespace Dryad
{

class MidiNote
{
public:

    MidiNote(U32 value = 0, ScoreTime duration = 0, ScoreTime startTime = 0)
        : _value(value)
        , _duration(duration)
        , _startTime(startTime)
    {
    }

    U32 GetValue() const
    {
        return _value;
    }

    ScoreTime GetDuration() const
    {
        return _duration;
    }

    ScoreTime GetStartTime() const
    {
        return _startTime;
    }

    U32 _value;
    ScoreTime _duration;
    ScoreTime _startTime;
};

} // namespace Dryad