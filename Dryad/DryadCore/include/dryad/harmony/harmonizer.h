#pragma once

#include "dryad/types.h"
#include "dryad/result.h"
#include "dryad/score/score.h"

namespace Dryad
{

class Instant;
class Score;

class Harmonizer
{
public:
    Harmonizer()
    {
    }

    static Result<> HarmonizeFrom(Score& score, Instant& instant)
    {
        return Success;
    }

};

} // namespace Dryad
