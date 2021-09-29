
// CRTP class helper
// Example:
// 
// template <class T>
// class Singleton : CrtpHelper<T, Singleton>
// {
// };
template <class CrtpChildClass, template<class> class CrtpClass>
class CrtpHelper
{
protected:
    // Convenience function to retrieve a reference to the inheriting class
    CrtpChildClass& getCrtpChild() { return static_cast<CrtpChildClass&>(*this); }
    //const InheritingClass& GetInheritingClass() { return static_cast<const InheritingClass&>(*this); }

private:
    // Private constructor resolves the ambiguity if more than once class implement the same crtp_class
    // Solves the diamond problem if a class inherits from multiple crtp_class
    friend CrtpClass<CrtpChildClass>;
    CrtpHelper() {}
};
