#pragma once

#include "dryad/types.h"
#include "dryad/harmony/note.h"
#include "dryad/harmony/degree.h"

namespace Dryad
{

class Scale
{
    Note _root;
    Degree _degrees[7];
};

} // namespace Dryad