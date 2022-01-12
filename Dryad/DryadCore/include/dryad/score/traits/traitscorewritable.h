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
        : _self(static_cast<T&>(*this))
    {
    }

public:

    ChildType<T>& appendChild()
    {
        List<ChildType<T>>& children = _self._children;
        children.emplace_back(_self);
        _self.setChanged(true);
        return children.back();
    }

    ChildType<T>& appendChild(ChildType<T>&& childToAppend)
    {
        auto& children = _self._children;
        children.emplace_back(childToAppend);
        _self.setChanged(true);
        return children.back();
    }

    T& append()
    {
        return _self._parent.appendChild();
    }

    T& append(T&& siblingToAppend)
    {
        return _self._parent.appendChild(std::forward<T>(siblingToAppend));
    }

private:

    T& _self;
};

}
