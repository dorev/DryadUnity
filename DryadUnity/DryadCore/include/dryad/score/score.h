#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/score/traits/scoretraits.h"
#include "dryad/score/phrase.h"

namespace Dryad
{

class Score : public ScoreHierarchy<Score>, public ScoreWritable<Score>
{
public:

    Score(Session& parentSession)
        : ExplorableBase(parentSession)
    {
    }

private:

    friend ExplorableBase;
    friend WritableBase;
    List<Phrase> _children;
};

} // namespace Dryad