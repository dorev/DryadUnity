#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct HarmonyNode;

struct HarmonyGraph : MonitorCount<HarmonyGraph>
{
    SharedPtr<Scale> scale;
    WeakPtr<Score> parentScore;
    Vector<SharedPtr<HarmonyNode>> nodes;
    Vector<Vector<WeakPtr<HarmonyNode>>> progressions;
};

} // namespace Dryad