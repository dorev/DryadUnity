#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"
#include "dryad/harmony/note.h"

namespace Dryad
{

class Voice
{
public:
    U32 GetId() const
    {
        return _descriptor.GetId();
    }

    const String& GetName()
    {
        return _descriptor.GetName();
    }

private:
    VoiceDescriptor _descriptor;
    Note _lowestNote;
    Note _highestNote;
    String _instrumentName;
    
};

} // namespace Dryad