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
    U32 GetId() const
    {
        return _descriptor.GetId();
    }

    const String& GetName() const
    {
        return _descriptor.GetName();
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
    ScoreTime _duration;
    Vector<MotifNote> _notes;
    MotifDescriptor _descriptor;
    Voice* _voice;
};

} // namespace Dryad