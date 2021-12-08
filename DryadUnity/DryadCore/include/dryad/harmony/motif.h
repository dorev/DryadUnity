#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"

namespace Dryad
{

class Motif
{
public:

    const MotifDescriptor& getDescriptor() { return _descriptor; }

private:

    MotifDescriptor _descriptor;
    VoiceDescriptor _voice;
};

} // namespace Dryad