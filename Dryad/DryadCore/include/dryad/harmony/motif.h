#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"
#include "dryad/harmony/motifnote.h"

namespace Dryad
{

class Voice;

class Motif
{
public:

    DELETE_DEFAULT_CONSTRUCTOR(Motif);

    Motif(const MotifDescriptor& descriptor)
        : _descriptor(descriptor)
        , _voice(nullptr)
        , _duration(0)
    {
    }

    U32 GetId() const
    {
        return _descriptor.GetId();
    }

    const String& GetName() const
    {
        return _descriptor.GetName();
    }

    const ScoreTime GetDuration() const
    {
        return _duration;
    }

    void SetWindowDuration(ScoreTime windowDuration)
    {
        _duration = windowDuration;
    }

    const Voice& GetVoice()
    {
        ASSERT(_voice != nullptr);
        return *_voice;
    }

    void SetVoice(Voice* voice)
    {
        _voice = voice;
    }

    const Vector<MotifNote>& GetNotes() const
    {
        return _notes;
    }

    void SetNotes(const Vector<MotifNote>& notes)
    {
        _notes = notes;
    }

private:
    Voice* _voice;
    ScoreTime _duration;
    Vector<MotifNote> _notes;
    MotifDescriptor _descriptor;
};

} // namespace Dryad
