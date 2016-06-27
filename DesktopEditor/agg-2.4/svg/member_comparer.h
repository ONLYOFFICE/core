//-----------------------------------------------------------------------------
#ifndef AGG_SVG_MEMBER_COMPARER_INCLUDED
#define AGG_SVG_MEMBER_COMPARER_INCLUDED
//-----------------------------------------------------------------------------
#include <functional> // for std::less
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    template <class T, class Type, class comparer>
    struct member_comparer 
    {
        //---------------------------------------------------------------------
        typedef Type    result_type;
        typedef T       class_type;
        //---------------------------------------------------------------------
        member_comparer(Type T::*PtrToMember, comparer comp = comparer()) 
            : m_PtrToMember(PtrToMember),
              m_comparer(comp)
        {}
        //---------------------------------------------------------------------
        bool operator()(class_type const& lhs, class_type const& rhs) const
        {
            return m_comparer(lhs.*m_PtrToMember, rhs.*m_PtrToMember);
        }
        //---------------------------------------------------------------------
        bool operator()(Type const& lhs, class_type const& rhs, int = 0) const
        {
            return m_comparer(lhs, rhs.*m_PtrToMember);
        }
        //---------------------------------------------------------------------
        bool operator()(class_type const& lhs, Type const& rhs) const
        {
            return m_comparer(lhs.*m_PtrToMember, rhs);
        }
        //---------------------------------------------------------------------
    private:
        Type T::*m_PtrToMember;
        comparer m_comparer;
    };
    //-------------------------------------------------------------------------
    template <class T, class Type, class comparer>
    inline 
    member_comparer<T, Type, comparer> 
    make_comparer(Type T::*PtrToMember, comparer comp)
    {
        return member_comparer<T, Type, comparer>(PtrToMember, comp);
    }    
    //-------------------------------------------------------------------------
    template <class T, class Type>
    inline 
    member_comparer<T, Type, std::less<Type> > 
    make_comparer(Type T::*PtrToMember)
    {
        return member_comparer<T, Type, std::less<Type> >
              (PtrToMember, std::less<Type>());
    }

} // namespace svg

} // namespace agg

#endif // #ifndef AGG_SVG_MEMBER_COMPARER_INCLUDED



