#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct HarmonyNode;
struct Measure;
struct Note;

struct Position : MonitorCount<Position>
{
    Position()
        : measureTime(0)
    {}

    Position(SharedPtr<Measure> parentMeasure, uint time = 0)
        : measureTime(time)
        , parentMeasure(parentMeasure)
    {}

    uint measureTime;
    Vector<SharedPtr<Note>> notes;
    SharedPtr<HarmonyNode> harmonyNode;
    WeakPtr<Measure> parentMeasure;
    WeakPtr<Position> next;
    WeakPtr<Position> previous;
};

} // namespace Dryad