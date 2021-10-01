#pragma once

#include "dryad/score/traits/traitscorehierarchy.h"
#include "dryad/score/traits/traitscorewritable.h"

namespace Dryad
{

template <class T>
class ScoreTraits
    : public CrtpHelper<T, ScoreTraits>
    , public ScoreHierarchy<T>
    , public ScoreWritable<T>
{

protected:

    // Only constructible through inheritance
    ScoreTraits(ParentType<T>& parent)
        : _session(parent.reachSession())
        , _parent(parent)
    {
    }

public:

    ~ScoreTraits()
    {
    }

protected:

    Session& _session;
    ParentType<T>& _parent;
    List<ChildType<T>> _children;
    friend class ScoreHierarchy<T>;
    friend class ScoreWritable<T>;
};

}
