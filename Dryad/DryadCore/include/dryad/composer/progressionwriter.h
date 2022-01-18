#pragma once

#include "dryad/types.h"
#include "dryad/error.h"
#include "dryad/result.h"

namespace Dryad
{

class Score;
class LandscapeGraph;

class ProgressionWriter
{

public:

    ProgressionWriter() = delete;
    ProgressionWriter(Score& score)
        : _score(score)
    {
    }

    Result<> TransitionToGraph(const LandscapeGraph& graph)
    {
        return Success;
    }


private:

    Score& _score;
};

} // namespace Dryad