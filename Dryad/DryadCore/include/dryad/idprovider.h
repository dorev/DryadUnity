#pragma once

#include "dryad/types.h"
#include "dryad/descriptors.h"

namespace Dryad
{
class IdProvider
{
public:

    IdProvider()
        : _voiceId(0)
        , _motifId(0)
        , _landscapeGraphId(0)
    {
    }

    VoiceDescriptor GetVoiceDescriptor(const String& voiceName)
    {
        return VoiceDescriptor(++_voiceId, voiceName);
    }

    MotifDescriptor CreateMotifDescriptor(const String& motifName)
    {
        return MotifDescriptor(++_motifId, motifName);
    }

    LandscapeGraphDescriptor CreateLandscapeGraphDescriptor(const String& landscapeName)
    {
        return LandscapeGraphDescriptor(++_landscapeGraphId, landscapeName);
    }

    LandscapeNodeId GetLandscapeNodeDescriptor(LandscapeGraphDescriptor& landscapeDescriptor)
    {
        return LandscapeNodeId(landscapeDescriptor);
    }

private:

    U32 _voiceId;
    U32 _motifId;
    U32 _landscapeGraphId;
};

} // namespace Dryad

