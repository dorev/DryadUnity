#pragma once

#include "types.h"

namespace Dryad
{

template <class T, class... Args>
SharedPtr<T> make(Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// CRTP class helper
template <typename InheritingClass, template<typename> typename CrtpClass>
class CrtpHelper
{
    // Convenience function to retrieve a reference to the inheriting class
    InheritingClass& GetInheritingClass() { return static_cast<InheritingClass&>(*this); }
    //const InheritingClass& GetInheritingClass() { return static_cast<const InheritingClass&>(*this); }

    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend CrtpClass<InheritingClass>;
    CrtpHelper() {}
};

struct Random
{
    template <class T>
    static T range(T min, T max)
    {
        if (min == max)
        {
            return min;
        }
        if (max < min)
        {
            std::swap(min, max);
        }

        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    }

    template <class T>
    static const auto& in(const T& container)
    {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
        return container[distribution(generator)];
    }

    static bool fiftyFifty();
};

template <class T>
bool contains(const T& value, const Vector<T>& vector)
{
    for (uint i = 0; i < vector.size(); ++i)
    {
        if (value == vector[i])
        {
            return true;
        }
    }

    return false;
}

template <class T>
typename std::enable_if<std::is_same<WeakPtr<T>, decltype(T::next)>::value, SharedPtr<T>>::type
next(SharedPtr<T> item)
{
    return item->next.lock();
}

template <class T>
typename std::enable_if<std::is_same<WeakPtr<T>, decltype(T::previous)>::value, SharedPtr<T>>::type
previous(SharedPtr<T> item)
{
    return item->previous.lock();
}

template <class T>
SharedPtr<T> clone(SharedPtr<T> item)
{
    return std::make_shared<T>(*item);
}

template <class T>
SharedPtr<T> last(const Vector<SharedPtr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.back();
}

template <class T>
SharedPtr<T> first(const Vector<SharedPtr<T>>& vector)
{
    if (vector.size() == 0)
    {
        return nullptr;
    }

    return vector.front();
}

void getEquivalentDurationPairs(uint duration, Vector<Pair<uint, uint>>& solutions);

bool isPowerOf2(uint value);

} // namespace Dryad
