#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Degree;
struct HarmonyGraph;

struct HarmonyNode : MonitorCount<HarmonyNode>
{
    HarmonyNode()
        : alteration(0)
        , inversion(0)
        , maxVisit(1)
        , modulation(0)
        , visitCount(0)
        , isEntry(false)
        , isExit(false)
        , name("")
    {}

    sint alteration;
    sint inversion;
    uint maxVisit;
    sint modulation;
    uint visitCount;
    bool isEntry;
    bool isExit;
    String name;
    Vector<WeakPtr<HarmonyNode>> edges;
    WeakPtr<HarmonyGraph> parentHarmonyGraph;
    WeakPtr<Degree> degree;
};

} // namespace Dryad