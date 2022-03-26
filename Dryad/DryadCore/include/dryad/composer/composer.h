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
		// lookup uncommitted motifs
		// check what motifs are currently there, compared to what should be present according to the context
	}

private:

	float _minStackedMotifOffsetPercentage;
	float _maxStackedMotifOffsetPercentage;


};

} // namespace Dryad