#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct MotifVariation;

struct Motif : MonitorCount<Motif>
{
    Vector<SharedPtr<MotifVariation>> variations;
    WeakPtr<Score> parentScore;
};

} // namespace Dryad