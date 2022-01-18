#pragma once

#include "dryad/types.h"
#include "dryad/harmony/landscapenode.h"
#include "dryad/harmony/scale.h"

namespace Dryad
{

class LandscapeGraph
{
    Scale scale;
    Vector<LandscapeNode> nodes;
    Vector<Vector<LandscapeNode*>> progressions;
};

} // namespace Dryad
