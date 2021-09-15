#pragma once

#include "types.h"
#include "constants.h"
#include "monitoring.h"

namespace Dryad
{

struct Position;
struct Phrase;
struct HarmonyNode;

struct Measure : MonitorCount<Measure>
{
    Measure(uint duration = Constants::Duration::Whole)
        : duration(duration)
    {}

    uint duration;
    Vector<SharedPtr<Position>> positions;
    Vector<SharedPtr<HarmonyNode>> progression;
    WeakPtr<Phrase> parentPhrase;
    WeakPtr<Measure> next;
    WeakPtr<Measure> previous;
};

} // namespace Dryad