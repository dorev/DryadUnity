#pragma once

#include "dryad/types.h"
#include "dryad/result.h"

namespace Dryad
{

class Position;
class Score;

class Harmonizer
{

public:

    Harmonizer() = delete;
    Harmonizer(Score& score)
        : _score(score)
    {
    }

    Result<> HarmonizeFrom(Position& position)
    {
        return Success;
    }

private:

    Score& _score;

};

} // namespace Dryad