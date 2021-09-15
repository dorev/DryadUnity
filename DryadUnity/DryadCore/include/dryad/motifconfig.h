#pragma once

#include "types.h"

namespace Dryad
{

struct MotifConfig
{
    MotifConfig(uint duration = 0, Energy melodicEnergy = 0, Energy rhythmicEnergy = 0)
        : duration(duration)
        , melodicEnergy(melodicEnergy)
        , minMelodicEnergy(1)
        , maxMelodicEnergy(8)
        , rhythmicEnergy(rhythmicEnergy)
    {}

    uint duration;
    Energy melodicEnergy;
    Energy minMelodicEnergy;
    Energy maxMelodicEnergy;
    Energy rhythmicEnergy;
};

} // namespace Dryad