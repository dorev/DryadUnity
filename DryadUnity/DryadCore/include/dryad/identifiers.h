#pragma once

#include "types.h"
#include "definitions.h"

namespace Dryad
{

class BaseDescriptor
{
public:

    BaseDescriptor()
        : _id(0)
    {
    }

    BaseDescriptor(U64 id, const String& name)
        : _id(id)
        , _name(name)
    {
    }

    virtual ~BaseDescriptor()
    {
    }

    U64 getId() const
    {
        return _id;
    }

    const String& getName() const
    {
        return _name;
    }

protected:

    const U64 _id;
    const String _name;
};

class VoiceDescriptor : public BaseDescriptor
{
public:

    VoiceDescriptor()
    {
    }

    VoiceDescriptor(U64 voiceId, const String& voiceName)
        : BaseDescriptor(voiceId, voiceName)
    {
    }

    bool operator<(const VoiceDescriptor& other) const
    {
        return getId() < other.getId();
    }
};

class MotifDescriptor : public BaseDescriptor
{
public:

    MotifDescriptor()
    {
    }

    MotifDescriptor(U64 motifId, const String& motifName)
        : BaseDescriptor(motifId, motifName)
    {
    }

    bool operator<(const MotifDescriptor& other) const
    {
        return getId() < other.getId();
    }
};

class HarmonyGraphDescriptor : public BaseDescriptor
{
public:

    HarmonyGraphDescriptor(U64 graphId, const String& graphName)
        : BaseDescriptor(graphId, graphName)
        , _nextNodeId(0)
    {
    }

    bool operator<(const HarmonyGraphDescriptor& other) const
    {
        return getId() < other.getId();
    }

protected:

    U64 nextNodeId()
    {
        return ++_nextNodeId;
    }

private:

    U64 _nextNodeId;
};

class HarmonyNodeId : public HarmonyGraphDescriptor
{
public:

    HarmonyNodeId(HarmonyGraphDescriptor& graphId)
        : HarmonyGraphDescriptor(graphId)
        , _nodeId(nextNodeId())
    {
    }

    U64 getNodeId() const
    {
        return _nodeId;
    }

    U64 getId() const
    {
        return _nodeId;
    }

    const HarmonyGraphDescriptor getGraphDescriptor() const
    {
        return *this;
    }

    bool operator<(const HarmonyNodeId& other) const
    {
        return getGraphDescriptor().getId() < other.getGraphDescriptor().getId()
            || (getGraphDescriptor().getId() == other.getGraphDescriptor().getId()
                && getId() < other.getId());
    }

private:

    U64 _nodeId;
};

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