#pragma once

#include "dryad/score/traits/traitscoreexplorable.h"
#include "dryad/score/traits/traitscorewritable.h"

namespace Dryad
{

template <class T>
class ScoreTraits
    : public CrtpHelper<T, ScoreTraits>
    , public ScoreExplorable<T>
    , public ScoreWritable<T>
{

protected:

    // Only constructible through inheritance
    ScoreTraits(ParentType<T>& parent)
        : _session(parent.GetSession())
        , _parent(parent)
        , _committed(false)
        , _changed(false)
    {
    }

public:

    ScoreTraits() = delete;
    ~ScoreTraits()
    {
    }

    void Commit()
    {
        _committed = true;
    }

    bool IsCommitted()
    {
        return _committed;
    }

    bool HasChanged()
    {
        return _changed;
    }

    void SetChanged(bool value = true)
    {
        _changed = value;

        if constexpr (!isType<Score>)
        {
            if(_changed)
                _parent.SetChanged(true);
        }
    }

protected:

    friend class ScoreExplorable<T>;
    friend class ScoreWritable<T>;

    Session& _session;
    ParentType<T>& _parent;
    List<ChildType<T>> _children;
    bool _committed;
    bool _changed;

    template <class U>
    static constexpr bool isType = std::is_same<T, U>::value;
};

}
