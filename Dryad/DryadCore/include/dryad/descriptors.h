#pragma once

#include "dryad/types.h"

namespace Dryad
{

class BaseDescriptor
{
public:
    BaseDescriptor()
        : _id(0)
        , _name("")
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

    bool IsValid()
    {
        return _id != 0 && _name != "";
    }

protected:
    U32 _id;
    String _name;
};

class VoiceDescriptor : public BaseDescriptor
{
public:
    VoiceDescriptor()
        : BaseDescriptor(0, "")
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
        : BaseDescriptor(0, "")
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
    LandscapeGraphDescriptor()
        : BaseDescriptor(0, "")
        , _nextNodeId(1)
    {
    }
    LandscapeGraphDescriptor(U32 landscapeId, const String& landscapeName)
        : BaseDescriptor(landscapeId, landscapeName)
        , _nextNodeId(1)
    {
    }

    bool operator<(const LandscapeGraphDescriptor& other) const
    {
        return GetId() < other.GetId();
    }

protected:
    U32 NextNodeId()
    {
        return _nextNodeId++;
    }

private:
    U32 _nextNodeId;
};

class LandscapeNodeId : public LandscapeGraphDescriptor
{
public:
    LandscapeNodeId()
        : LandscapeGraphDescriptor()
        , _nodeId(0)
    {
    }

    LandscapeNodeId(LandscapeGraphDescriptor& graphId)
        : LandscapeGraphDescriptor(graphId)
        , _nodeId(NextNodeId())
    {
    }

    U32 GetId() const
    {
        return _nodeId;
    }

    const LandscapeGraphDescriptor GetGraphDescriptor() const
    {
        return *this;
    }

    bool operator<(const LandscapeNodeId& other) const
    {
        return GetGraphDescriptor().GetId() < other.GetGraphDescriptor().GetId()
            || (GetGraphDescriptor().GetId() == other.GetGraphDescriptor().GetId() && GetId() < other.GetId());
    }

private:
    U32 _nodeId;

    // Hide base class GetId method
    using BaseDescriptor::GetId;
};

} // namespace Dryad

