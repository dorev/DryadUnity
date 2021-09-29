#pragma once

#include "dryad/score/scorecommon.h"
#include "dryad/crtphelper.h"

namespace Dryad
{

template <class T>
class ScoreHierarchy : public CrtpHelper<T, ScoreHierarchy>
{

protected:

    // Only constructible through inheritance
    ScoreHierarchy(ParentType<T>& parent)
        : _parent(parent)
        , _session(parent.reachSession())
    {
    }

public:

    ScoreHierarchy() = delete;
    
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
        return _session;
    }

    template <U32 generation = 1>
    auto* getParent()
    {
        if constexpr (generation == 0)
            return this;
        else if constexpr (generation == 1)
            return &_parent;
        else
            return getParent(--generation);
    }

    List<ChildType<T>>& getChildren()
    {
        return CrtpBase::getCrtpChild()._children;
    }

    List<T>& getSiblings()
    {
        return getParent()->ScoreHierarchy<ParentType<T>>::getChildren();
    }

    template <U32 generation = 1>
    auto* getFirstChild()
    {
        if constexpr (generation == 0)
            return this;
        if constexpr (_typeId == ScoreElementTypeId::Note)
            return static_cast<Note*>(nullptr);
        else if constexpr (generation == 1)
        {
            auto children = getChildren();
            if(children.empty())
                return static_cast<ChildType<T>*>(nullptr);

            return &*children.begin();
        }
        else
            return getFirstChild(--generation);
    }

    template <ScoreElementTypeId relativeTypeId>
    auto* moveToHierarchy()
    {
        constexpr S32 targetGeneration = GetGenerationDelta<_typeId, relativeTypeId>;

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
        auto* targetRelative = moveToHierarchy<GetTypeId<IteratedScoreElementType>>();

        if(targetRelative != nullptr)
            return targetRelative->prev();
        else
            return nullptr;
    }

    template <class IteratedScoreElementType>
    IteratedScoreElementType* next()
    {
        auto* targetRelative = moveToHierarchy<GetTypeId<IteratedScoreElementType>>();

        if(targetRelative != nullptr)
            return targetRelative->next();
        else
            return nullptr;
    }

protected:

    using ExplorableBase = typename ScoreHierarchy<T>;
    ParentType<T>& _parent;
    Session& _session;
    static constexpr ScoreElementTypeId _typeId = GetTypeId<T>;

private:

    using CrtpBase = typename CrtpHelper<T, ScoreHierarchy>;
};

}
