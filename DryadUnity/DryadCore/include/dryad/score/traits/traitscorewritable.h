#pragma once

#include "dryad/score/traits/traitsutils.h"

namespace Dryad
{

template<class T>
class ScoreWritable : public CrtpHelper<T, ScoreWritable>
{

protected:

    // Only constructible through inheritance
    ScoreWritable()
    {
    }

public:

    ChildType<T>& appendChild()
    {
        auto& children = CrtpBase::getCrtpChild()._children;
        children.emplace_back(CrtpBase::getCrtpChild());
        getScoreTraitsBase().setChanged(true);
        return children.back();
    }

    ChildType<T>& appendChild(ChildType<T>&& childToAppend)
    {
        auto& children = CrtpBase::getCrtpChild()._children;
        children.emplace_back(childToAppend);
        getScoreTraitsBase().setChanged(true);
        return children.back();
    }

    T& append()
    {
        return CrtpBase::getCrtpChild()._parent.appendChild();
    }

    T& append(T&& siblingToAppend)
    {
        return CrtpBase::getCrtpChild()._parent.appendChild(std::forward<T>(siblingToAppend));
    }

private:

    ScoreTraits<T>& getScoreTraitsBase()
    {
        return static_cast<ScoreTraits<T>&>(CrtpBase::getCrtpChild());
    }

    using CrtpBase = typename CrtpHelper<T, ScoreWritable>;
};

}
