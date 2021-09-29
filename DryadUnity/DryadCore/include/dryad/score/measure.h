#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/position.h"

namespace Dryad
{

class Measure : public ScoreHierarchy<Measure>, public ScoreWritable<Measure>
{
public:

    Measure(Phrase& parentPhrase)
        : ExplorableBase(parentPhrase)
    {
    }

private:

    friend ExplorableBase;
    friend WritableBase;
    List<Position> _children;
};

}
