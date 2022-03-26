#pragma once

#include "dryad/types.h"
#include "dryad/constants.h"

#include "dryad/harmony/harmonycontext.h"
#include "dryad/score/score.h"

namespace Dryad
{

class Composer
{
public:

    Result<> Compose(const HarmonyContext& harmonyContext, Score& score, ScoreTime commitLength)
    {
        HarmonyContext::Snapshot contextSnapshot = harmonyContext.GetSnapshot();

        // lookup uncommitted motifs
        // check what motifs are currently there, compared to what should be present according to the context


        // notes added to score should always have a motifInstanceId to know what note must be kept or removed when motifs changes
    }

private:

    ScoreTime _maxStackedMotifOffset;
    U8 _maxNotePerInstant;
    bool _completeLandscapeChord;

};

} // namespace Dryad