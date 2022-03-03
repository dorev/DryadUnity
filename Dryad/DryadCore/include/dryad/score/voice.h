#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"

namespace Dryad
{

class Voice
{
public:

    const VoiceDescriptor& getDescriptor() const
    {
        return _descriptor;
    }

private:

    VoiceDescriptor _descriptor;
};

} // namespace Dryad