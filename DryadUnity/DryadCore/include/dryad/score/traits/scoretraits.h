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
        , _changed(false)
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

    bool hasChanged()
    {
        return _changed;
    }

    void setChanged(bool value = true)
    {
        _changed = value;
    }

protected:

    Session& _session;
    ParentType<T>& _parent;
    bool _committed;
    bool _changed;
    List<ChildType<T>> _children;
    friend class ScoreHierarchy<T>;
    friend class ScoreWritable<T>;
};

}
