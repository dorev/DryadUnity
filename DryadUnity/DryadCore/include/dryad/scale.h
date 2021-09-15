#pragma once

#include "types.h"
#include "monitoring.h"

namespace Dryad
{

struct Degree;

struct Scale : MonitorCount<Scale>
{
    String name;
    Vector<SharedPtr<Degree>> degrees;
};

} // namespace Dryad