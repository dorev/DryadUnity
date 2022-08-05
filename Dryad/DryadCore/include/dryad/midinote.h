#pragma once

#include "dryad/types.h"
#include "dryad/definitions.h"

namespace Dryad
{

class MidiNote
{
public:
    DELETE_DEFAULT_CONSTRUCTOR(MidiNote);

    MidiNote(U32 value, U32 voiceId, ScoreTime duration, ScoreTime startTime)
        : _value(value)
        , _voiceId(voiceId)
        , _duration(duration)
        , _startTime(startTime)
    {
    }

    U32 GetValue() const
    {
        return _value;
    }

    U32 GetVoiceId() const
    {
        return _voiceId;
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
    U32 _voiceId;
    ScoreTime _duration;
    ScoreTime _startTime;
};

} // namespace Dryad
