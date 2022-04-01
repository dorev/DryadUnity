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

    Composer()
        : _maxStackedMotifOffsetPercentage(50)
        , _maxNotePerInstant(MAX_NOTES_PER_INSTANT)
        , _completeLandscapeChord(false)
        , _nextMotifInstanceId(0)
    {
    }

    Result<> Compose(const HarmonyContext& harmonyContext, Score& score, ScoreTime commitLength)
    {
        if (commitLength == 0)
            return { ErrorCode::NothingToCommit };
        if (harmonyContext.HasNoMotifs())
            return { ErrorCode::NoMotifAvailable };

        // in what measure are we?
        // do I have new motif instances to write?
        //  --> what is the score time of the measures beginning (done!)
        //  --> need to compare current score context vs current harmony context
        //      * how to identify the context on the score?
        //      --> Instant gives scale and landscapenode
        //      --> ScoreNote will list the motifs present
        //  --> max instances of motif per measure?



        Instant* instant = score.GetFirstUncommittedInstant();
        
        // compare previous context to current
        


        // lookup uncommitted motifs
        // check what motifs are currently there, compared to what should be present according to the context


        // notes added to score should always have a motifInstanceId to know what note must be kept or removed when motifs changes
        return Success;
    }

private:

    HarmonyContextSnapshot _previousContextSnapshot;
    HarmonyContextSnapshot _currentContextSnapshot;
    U8 _maxStackedMotifOffsetPercentage;
    U8 _maxNotePerInstant;
    bool _completeLandscapeChord;
    U32 _nextMotifInstanceId;
};

} // namespace Dryad