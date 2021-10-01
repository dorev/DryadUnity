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

    Score& reachScore()
    {
        if constexpr(_typeId == ScoreElementTypeId::Score)
            return *this;
        else
            return reachSession().getScore();
    }

    Session& reachSession()
    {
        return CrtpBase::getCrtpChild()._parent.reachSession();
    }

    template <U32 distance = 1>
    auto* getParent()
    {
        if constexpr (distance == 0)
            return this;
        else if constexpr (distance == 1)
            return &CrtpBase::getCrtpChild()._parent;
        else
            return getParent(--distance);
    }

    List<ChildType<T>>& getChildren()
    {
        return CrtpBase::getCrtpChild()._children;
    }

    List<T>& getSiblings()
    {
        return getParent()->ScoreHierarchy<ParentType<T>>::getChildren();
    }

    template <U32 distance = 1>
    auto* getFirstChild()
    {
        if constexpr (distance == 0)
            return this;
        if constexpr (_typeId == ScoreElementTypeId::Note)
            return static_cast<Note*>(nullptr);
        else if constexpr (distance == 1)
        {
            auto children = getChildren();
            if(children.empty())
                return static_cast<ChildType<T>*>(nullptr);

            return &*children.begin();
        }
        else
            return getFirstChild(--distance);
    }

    template <ScoreElementTypeId otherTypeId>
    auto* moveToHierarchy()
    {
        constexpr S32 targetGeneration = GetHierarchyDistance<_typeId, otherTypeId>;

        if constexpr (targetGeneration == 0)
            return this;
        else if constexpr (targetGeneration < 0)
            return getParent<targetGeneration>();
        else
            return getFirstChild<targetGeneration>();
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

    template <class IteratedScoreElementType>
    IteratedScoreElementType* prev()
    {
        auto* target = moveToHierarchy<GetTypeId<IteratedScoreElementType>>();

        if(target != nullptr)
            return target->prev();
        else
            return nullptr;
    }

    template <class IteratedScoreElementType>
    IteratedScoreElementType* next()
    {
        auto* target = moveToHierarchy<GetTypeId<IteratedScoreElementType>>();

        if(target != nullptr)
            return target->next();
        else
            return nullptr;
    }

private:

    using CrtpBase = typename CrtpHelper<T, ScoreHierarchy>;
    static constexpr ScoreElementTypeId _typeId = GetTypeId<T>;
};

}
