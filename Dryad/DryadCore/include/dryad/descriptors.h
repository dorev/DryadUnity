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

    BaseDescriptor(U32 id, const String& name)
        : _id(id)
        , _name(name)
    {
    }

    virtual ~BaseDescriptor()
    {
    }

    U32 GetId() const
    {
        return _id;
    }

    const String& GetName() const
    {
        return _name;
    }

protected:

    U32 _id;
    String _name;
};

class VoiceDescriptor : public BaseDescriptor
{
public:

    VoiceDescriptor()
    {
    }

    VoiceDescriptor(U32 voiceId, const String& voiceName)
        : BaseDescriptor(voiceId, voiceName)
    {
    }

    bool operator<(const VoiceDescriptor& other) const
    {
        return GetId() < other.GetId();
    }
};

class MotifDescriptor : public BaseDescriptor
{
public:

    MotifDescriptor()
    {
    }

    MotifDescriptor(U32 motifId, const String& motifName)
        : BaseDescriptor(motifId, motifName)
    {
    }

    bool operator<(const MotifDescriptor& other) const
    {
        return GetId() < other.GetId();
    }
};

class LandscapeGraphDescriptor : public BaseDescriptor
{
public:

    LandscapeGraphDescriptor(U32 landscapeId, const String& landscapeName)
        : BaseDescriptor(landscapeId, landscapeName)
        , _nextNodeId(0)
    {
    }

    bool operator<(const LandscapeGraphDescriptor& other) const
    {
        return GetId() < other.GetId();
    }

protected:

    U32 NextNodeId()
    {
        return ++_nextNodeId;
    }

private:

    U32 _nextNodeId;
};

class LandscapeNodeId : public LandscapeGraphDescriptor
{
public:

    LandscapeNodeId(LandscapeGraphDescriptor& graphId)
        : LandscapeGraphDescriptor(graphId)
        , _nodeId(NextNodeId())
    {
    }

    U32 GetId() const
    {
        return _nodeId;
    }

    const LandscapeGraphDescriptor getGraphDescriptor() const
    {
        return *this;
    }

    bool operator<(const LandscapeNodeId& other) const
    {
        return getGraphDescriptor().GetId() < other.getGraphDescriptor().GetId()
            || (getGraphDescriptor().GetId() == other.getGraphDescriptor().GetId() && GetId() < other.GetId());
    }

private:

    using BaseDescriptor::GetId;
    U32 _nodeId;
};

} // namespace Dryad
