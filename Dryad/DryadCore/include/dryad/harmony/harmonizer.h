#pragma once

#include "dryad/types.h"
#include "dryad/result.h"
#include "dryad/score/score.h"

namespace Dryad
{

class Instant;

class Harmonizer
{
public:
    Harmonizer() = delete;
    Harmonizer(Score& score)
        : _score(score)
    {
    }

    Result<> HarmonizeFrom(Instant& instant)
    {
        return Success;
    }

private:
    Score& _score;

};

} // namespace Dryad