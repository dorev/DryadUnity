#pragma once

#include "dryad/types.h"
#include "dryad/harmony/landscapenode.h"
#include "dryad/harmony/scale.h"

namespace Dryad
{

struct Edge
{
    LandscapeNode* to;
    LandscapeNode* from;
};

class LandscapeGraph
{
public:

    LandscapeGraph(const LandscapeGraphDescriptor& descriptor)
        : _descriptor(descriptor)
        , _nodes()
        , _edges()
    {
    }

    U32 GetId() const
    {
        return _descriptor.GetId();
    }

    const String& GetName() const
    {
        return _descriptor.GetName();
    }

private:
    LandscapeGraphDescriptor _descriptor;
    Map<String, LandscapeNode> _nodes;
    Vector<Edge> _edges;
};

} // namespace Dryad
