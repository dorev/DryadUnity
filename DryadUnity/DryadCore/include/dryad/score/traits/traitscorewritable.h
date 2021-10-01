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
        auto& children = getChildren();
        children.emplace_back(CrtpBase::getCrtpChild());
        return children.back();
    }

    ChildType<T>& appendChild(ChildType<T>&& childToAppend)
    {
        auto& children = getChildren();
        children.emplace_back(childToAppend);
        return children.back();
    }

    T& append()
    {
        return getParent().appendChild();
    }

    T& append(T&& siblingToAppend)
    {
        return getParent().appendChild(std::forward<T>(siblingToAppend));
    }

private:

    using CrtpBase = typename CrtpHelper<T, ScoreWritable>;

    List<ChildType<T>>& getChildren()
    {
        return CrtpBase::getCrtpChild()._children;
    }

    ParentType<T>& getParent()
    {
        return CrtpBase::getCrtpChild()._parent;
    }
};

}
