#pragma once

#include "types.h"
#include "harmonynode.h"
#include "scale.h"

namespace Dryad
{

class HarmonyGraph
{
    Scale scale;
    Vector<HarmonyNode> nodes;
    Vector<Vector<HarmonyNode*>> progressions;
};

} // namespace Dryad
