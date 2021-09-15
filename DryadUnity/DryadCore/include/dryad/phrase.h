#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct HarmonyNode;
struct Measure;
struct Score;

struct Phrase : MonitorCount<Phrase>
{
    Vector<SharedPtr<Measure>> measures;
    Vector<WeakPtr<HarmonyNode>> progression;
    WeakPtr<Score> parentScore;
    WeakPtr<Phrase> next;
    WeakPtr<Phrase> previous;
};

} // namespace Dryad