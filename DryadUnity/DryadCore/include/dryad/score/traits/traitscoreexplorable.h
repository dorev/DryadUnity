#pragma once

#include "dryad/score/traits/traitsutils.h"

namespace Dryad
{

template <class T>
class ScoreExplorable : public CrtpHelper<T, ScoreExplorable>
{

protected:

    // Only constructible through inheritance
    ScoreExplorable()
        : _self(static_cast<T&>(*this))
        , _lastUncommittedChildCache(nullptr)
    {
    }

public:

    virtual ~ScoreExplorable()
    {
    }

    Score& getScore()
    {
        if constexpr(isType<Score>)
            return *this;
        else
            return getSession().getScore();
    }

    Session& getSession()
    {
        return _self._parent.getSession();
    }

    ParentType<T>* getParent()
    {
        if constexpr (isType<Score>)
            return nullptr;
        else
            return &_self._parent;
    }

    ChildType<T>* getLastUncommittedChild()
    {
        if(getChildren().empty())
            return nullptr;

        if(!_self.hasChanged())
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
        return _self._children;
    }

    List<T>& getSiblings()
    {
        return getParent()->getChildren();
    }

    T* prev()
    {
        if constexpr (!isType<Score>)
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
        if constexpr (!isType<Score>)
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

    T& _self;
    ChildType<T>* _lastUncommittedChildCache;

    template <class U>
    static constexpr bool isType = std::is_same<T, U>::value;
};

}
