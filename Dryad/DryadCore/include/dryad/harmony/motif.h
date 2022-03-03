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

    const MotifDescriptor& getDescriptor() const
    {
        return _descriptor;
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

    Vector<MotifNote> _notes;
    ScoreTime duration;
    MotifDescriptor _descriptor;
    Voice* _voice;
};

} // namespace Dryad