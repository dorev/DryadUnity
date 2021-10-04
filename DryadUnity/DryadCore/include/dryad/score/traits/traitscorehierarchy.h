#pragma once

#include "dryad/score/traits/traitsutils.h"

namespace Dryad
{


template <class T>
class ScoreTraits;

template <class T>
class ScoreHierarchy : public CrtpHelper<T, ScoreHierarchy>
{

protected:

    // Only constructible through inheritance
    ScoreHierarchy()
        : _lastUncommittedChildCache(nullptr)
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

        ScoreTraits<T>& scoreTraits = getScoreTraitsBase();

        if(!scoreTraits.hasChanged())
            return _lastUncommittedChildCache;

        for (auto& child : getChildren())
        {
            if (!child.isCommitted())
            {
                _lastUncommittedChildCache = &child;
                return &child;
            }
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
        if constexpr (GetTypeId<T> != ScoreElementTypeId::Score)
        {
            List<T>& siblings = getSiblings();

            for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
            {
                if (&*sibling == this)
                {
                    if (sibling == siblings.begin())
                    {
                        // Check previous parent last child
                        ParentType<T>* parent = sibling->getParent();

                        if(parent == nullptr)
                            break;

                        ParentType<T>* previousParent = parent->prev();

                        if(previousParent == nullptr || previousParent->getChildren().empty())
                            break;

                        return &previousParent->getChildren().back();
                    }
                    else
                        return &*(--sibling);
                }
            }
        }

        return nullptr;
    }

    T* next()
    {
        if constexpr (GetTypeId<T> != ScoreElementTypeId::Score)
        {
            List<T>& siblings = getSiblings();

            for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
            {
                if (&*sibling == this)
                {
                    if (++sibling == siblings.end())
                    {
                        ParentType<T>* parent = sibling->getParent();

                        if(parent == nullptr)
                            break;

                        ParentType<T>* nextParent = parent->prev();

                        if(nextParent == nullptr || nextParent->getChildren().empty())
                            break;

                        return &nextParent->getChildren().front();
                    }
                    else
                        return &*(sibling);
                }
            }
        }

        return nullptr;
    }

private:

    ScoreTraits<T>& getScoreTraitsBase()
    {
        return static_cast<ScoreTraits<T>&>(CrtpBase::getCrtpChild());
    }

    using CrtpBase = typename CrtpHelper<T, ScoreHierarchy>;
    static constexpr ScoreElementTypeId _typeId = GetTypeId<T>;
    ChildType<T>* _lastUncommittedChildCache;
};

}
