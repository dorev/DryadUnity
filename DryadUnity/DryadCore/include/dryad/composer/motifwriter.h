#pragma once

#include "dryad/types.h"
#include "dryad/error.h"
#include "dryad/result.h"

namespace Dryad
{

class Score;
class Motif;

class MotifWriter
{

public:

    MotifWriter() = delete;
    MotifWriter(Score& score)
        : _score(score)
    {
    }

    Result<> decreaseMotifPresence(const Motif& motif, U32 amount = 1)
    {
        return Success;
    }

    Result<> increaseMotifPresence(const Motif& motif, U32 amount = 1)
    {
        return Success;
    }

private:

    Score& _score;
};

} // namespace Dryad