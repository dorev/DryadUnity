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

    ChildType<T>& AppendChild()
    {
        List<ChildType<T>>& children = _self._children;
        children.emplace_back(_self);
        _self.SetChanged(true);
        return children.back();
    }

    ChildType<T>& AppendChild(ChildType<T>&& childToAppend)
    {
        auto& children = _self._children;
        children.emplace_back(childToAppend);
        _self.SetChanged(true);
        return children.back();
    }

    T& Append()
    {
        return _self._parent.AppendChild();
    }

    T& Append(T&& siblingToAppend)
    {
        return _self._parent.AppendChild(std::forward<T>(siblingToAppend));
    }

private:

    T& _self;
};

}
