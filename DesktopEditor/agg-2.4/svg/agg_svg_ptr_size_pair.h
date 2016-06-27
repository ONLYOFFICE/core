#ifndef AGG_SVG_PTR_SIZE_PAIR_INCLUDE
#define AGG_SVG_PTR_SIZE_PAIR_INCLUDE
//-----------------------------------------------------------------------------
#include "agg_basics.h"
#include <cassert>
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    struct ptr_size 
    {
        ptr_size(const void * ptr, agg::int32u size) 
            : m_ptr(static_cast<const agg::int8u *>(ptr)), 
              m_size(size) 
        {
        }

        const agg::int8u * m_ptr;
        agg::int32u        m_size;
    };
    //-------------------------------------------------------------------------
    struct ptr_size_short 
    {
        ptr_size_short(const void * ptr, agg::int32u size) 
            : m_ptr(static_cast<const agg::int8u *>(ptr)), 
              m_size(static_cast<agg::int8u>(size)) 
        {
            assert(m_size == size);
        }

        const agg::int8u * m_ptr;
        agg::int8u         m_size;
    };
	//-------------------------------------------------------------------------

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_PTR_SIZE_PAIR_INCLUDE

