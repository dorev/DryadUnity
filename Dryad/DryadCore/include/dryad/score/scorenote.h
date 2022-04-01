#pragma once

#include "dryad/midinote.h"
#include "dryad/types.h"
#include "dryad/harmony/motif.h"
#include "dryad/score/voice.h"
#include "dryad/harmony/note.h"

namespace Dryad
{
class ScoreNote
{
public:
    ScoreNote()
        : _midi(0)
        , _duration(0)
        , _voice(nullptr)
        , _motif(nullptr)
        , _motifInstanceId(0)
    {
    }

    U32 GetMidiValue() const
    {
        return _midi;
    }

    ScoreTime GetDuration() const
    {
        return _duration;
    }

    U32 GetVoiceId() const
    {
        ASSERT(_voice != nullptr);
        return _voice->GetId();
    }

    U32 GetMotifId() const
    {
        ASSERT(_motif != nullptr);
        return _motif->GetId();
    }

private:
    U32 _midi;
    ScoreTime _duration;
    Voice* _voice;
    Motif* _motif;
    U32 _motifInstanceId;
};

}
