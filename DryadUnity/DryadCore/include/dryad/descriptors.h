#pragma once

#include "dryad/types.h"

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

    U64 _id;
    String _name;
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

    using BaseDescriptor::getId;
    U64 _nodeId;
};

} // namespace Dryad
