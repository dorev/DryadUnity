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
        : _session(parent.getSession())
        , _parent(parent)
        , _committed(false)
    {
    }

public:

    ~ScoreTraits()
    {
    }

    void commit()
    {
        _committed = true;
    }

    bool isCommitted()
    {
        return _committed;
    }

protected:

    Session& _session;
    ParentType<T>& _parent;
    bool _committed;
    List<ChildType<T>> _children;
    friend class ScoreHierarchy<T>;
    friend class ScoreWritable<T>;
};

}
