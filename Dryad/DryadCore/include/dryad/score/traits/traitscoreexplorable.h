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

    Score& GetScore()
    {
        if constexpr(isType<Score>)
            return *this;
        else
            return GetSession().GetScore();
    }

    Session& GetSession()
    {
        return _self._parent.GetSession();
    }

    ParentType<T>* GetParent()
    {
        if constexpr (isType<Score>)
            return nullptr;
        else
            return &_self._parent;
    }

    ChildType<T>* GetLastUncommittedChild()
    {
        if(GetChildren().empty())
            return nullptr;

        if(!_self.HasChanged())
            return _lastUncommittedChildCache;

        for (auto& child : GetChildren())
        {
            if (!child.IsCommitted())
            {
                _lastUncommittedChildCache = &child;
                return &child;
            }
        }

        return nullptr;
    }

    List<ChildType<T>>& GetChildren()
    {
        return _self._children;
    }

    List<T>& GetSiblings()
    {
        return GetParent()->GetChildren();
    }

    T* Prev()
    {
        if constexpr (!isType<Score>)
        {
            List<T>& siblings = GetSiblings();

            for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
            {
                if (&*sibling == this)
                {
                    if (sibling == siblings.begin())
                    {
                        // Check previous parent last child
                        ParentType<T>* parent = sibling->GetParent();

                        if(parent == nullptr)
                            break;

                        ParentType<T>* previousParent = parent->Prev();

                        if(previousParent == nullptr || previousParent->GetChildren().empty())
                            break;

                        return &previousParent->GetChildren().back();
                    }
                    else
                        return &*(--sibling);
                }
            }
        }

        return nullptr;
    }

    T* Next()
    {
        if constexpr (!isType<Score>)
        {
            List<T>& siblings = GetSiblings();

            for(auto sibling = siblings.begin(); sibling != siblings.end(); sibling++)
            {
                if (&*sibling == this)
                {
                    if (++sibling == siblings.end())
                    {
                        ParentType<T>* parent = sibling->GetParent();

                        if(parent == nullptr)
                            break;

                        ParentType<T>* nextParent = parent->Prev();

                        if(nextParent == nullptr || nextParent->GetChildren().empty())
                            break;

                        return &nextParent->GetChildren().front();
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
