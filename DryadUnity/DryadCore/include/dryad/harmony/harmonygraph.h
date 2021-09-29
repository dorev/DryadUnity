#pragma once

#include "dryad/types.h"
#include "dryad/harmony/harmonynode.h"
#include "dryad/harmony/scale.h"

namespace Dryad
{

class HarmonyGraph
{
    Scale scale;
    Vector<HarmonyNode> nodes;
    Vector<Vector<HarmonyNode*>> progressions;
};

} // namespace Dryad
