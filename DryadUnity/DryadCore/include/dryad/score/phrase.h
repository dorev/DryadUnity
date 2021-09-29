#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/measure.h"

namespace Dryad
{

class Phrase : public ScoreHierarchy<Phrase>, public ScoreWritable<Phrase>
{
public:

    Phrase(Score& parentScore)
        : ExplorableBase(parentScore)
    {
    }

private:

    friend ExplorableBase;
    friend WritableBase;
    List<Measure> _children;
};

}
