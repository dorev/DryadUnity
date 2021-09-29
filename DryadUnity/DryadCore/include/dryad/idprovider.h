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
        , _harmonyGraphId(0)
    {
    }

    VoiceDescriptor nextVoice(const String& voiceName)
    {
        return VoiceDescriptor(++_voiceId, voiceName);
    }

    MotifDescriptor nextMotif(const String& motifName)
    {
        return MotifDescriptor(++_motifId, motifName);
    }

    HarmonyGraphDescriptor nextHarmonyGraph(const String& graphName)
    {
        return HarmonyGraphDescriptor(++_harmonyGraphId, graphName);
    }

    HarmonyNodeId nextHarmonyNodeId(HarmonyGraphDescriptor& graphId)
    {
        return HarmonyNodeId(graphId);
    }

private:

    U64 _voiceId;
    U64 _motifId;
    U64 _harmonyGraphId;
};

} // namespace Dryad
