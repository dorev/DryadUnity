#pragma once

#include "dryad/types.h"
#include "dryad/error.h"
#include "dryad/result.h"

namespace Dryad
{

class Score;
class HarmonyGraph;

class ProgressionWriter
{

public:

    ProgressionWriter() = delete;
    ProgressionWriter(Score& score)
        : _score(score)
    {
    }

    Result<> transitionToGraph(const HarmonyGraph& graph)
    {
        return Success;
    }


private:

    Score& _score;
};

} // namespace Dryad