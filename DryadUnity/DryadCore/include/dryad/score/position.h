#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/note.h"

namespace Dryad
{

class Position : ScoreHierarchy<Position>, public ScoreWritable<Position>
{
public:

    Position(Measure& parentMeasure)
        : ExplorableBase(parentMeasure)
    {
    }

private:

    friend ExplorableBase;
    friend WritableBase;
    List<Note> _children;
};

}
