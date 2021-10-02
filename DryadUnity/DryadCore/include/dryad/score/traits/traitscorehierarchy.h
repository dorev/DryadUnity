#pragma once

#include "dryad/score/traits/traitsutils.h"

namespace Dryad
{

template <class T>
class ScoreWritable;

template <class T>
class ScoreHierarchy : public CrtpHelper<T, ScoreHierarchy>
{

protected:

    // Only constructible through inheritance
    ScoreHierarchy()
    {
    }

public:

    virtual ~ScoreHierarchy()
    {
    }

    Score& getScore()
    {
        if constexpr(_typeId == ScoreElementTypeId::Score)
            return *this;
        else
            return getSession().getScore();
    }

    Session& getSession()
    {
        return CrtpBase::getCrtpChild()._parent.getSession();
    }

    ParentType<T>* getParent()
    {
        if constexpr (_typeId == ScoreElementTypeId::Score)
            return nullptr;
        else
            return &CrtpBase::getCrtpChild()._parent;
    }

    ChildType<T>* getLastUncommittedChild()
    {
        if(getChildren().empty())
            return nullptr;

        for (auto& child : getChildren())
        {
            if(!child.isCommitted())
                return &child;
        }

        return nullptr;
    }

    List<ChildType<T>>& getChildren()
    {
        return CrtpBase::getCrtpChild()._children;
    }

    List<T>& getSiblings()
    {
        return getParent()->ScoreHierarchy<ParentType<T>>::getChildren();
    }

    T* prev()
    {
        List<T>& siblings = getSiblings();

        for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
        {
            if (&*sibling == this)
            {
                if(sibling == siblings.begin())
                    break;
                else
                    return &*(--sibling);
            }
        }

        return nullptr;
    }

    T* next()
    {
        List<T>& siblings = getSiblings();

        for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
        {
            if (&*sibling == this)
            {
                if(++sibling == siblings.end())
                    break;
                else
                    return &*(sibling);
            }
        }

        return nullptr;
    }

private:

    using CrtpBase = typename CrtpHelper<T, ScoreHierarchy>;
    static constexpr ScoreElementTypeId _typeId = GetTypeId<T>;
};

}
