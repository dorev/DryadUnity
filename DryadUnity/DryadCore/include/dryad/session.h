#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Session : MonitorCount<Session>
{
    uint currentTime;
    SharedPtr<HarmonyGraph> currentGraph;

};

} // namespace Dryad