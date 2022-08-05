#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"

namespace Dryad
{

class MotifNote
{
public:
    MotifNote()
        : _tonicOffset(0)
        , _startTime(0)
        , _duration(0)
    {
    }

    MotifNote(S32 tonicOffset, ScoreTime startTime, ScoreTime duration)
        : _tonicOffset(tonicOffset)
        , _startTime(startTime)
        , _duration(duration)
    {
    }

    const S32 GetTonicOffset() const
    {
        return _tonicOffset;
    }

    const ScoreTime GetStartTime() const
    {
        return _startTime;
    }

    const ScoreTime GetDuration() const
    {
        return _duration;
    }

private:
    S32 _tonicOffset;
    ScoreTime _startTime;
    ScoreTime _duration;
};

} // namespace Dryad
