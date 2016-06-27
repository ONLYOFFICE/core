#ifndef AGG_SVG_IOMEMSTREAM_INCLUDE
#define AGG_SVG_IOMEMSTREAM_INCLUDE
//-----------------------------------------------------------------------------
#include "agg_svg_basics.h"
#include "agg_svg_data_accessor.h"
#include "agg_svg_ptr_size_pair.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class iomemstream
    {
    public:
        //---------------------------------------------------------------------
        void clear();
        //---------------------------------------------------------------------
        agg::int32u size() const;
        //---------------------------------------------------------------------
        data_container const & buffer() const;
        data_container  & buffer() ;
        //---------------------------------------------------------------------
        iomemstream& operator<<(agg::int8u);
        iomemstream& operator<<(agg::int32u);
        iomemstream& operator<<(const ptr_size &);
        iomemstream& operator<<(const ptr_size_short &);
        iomemstream& operator<<(data_accessor_type);
        //---------------------------------------------------------------------
        iomemstream& write(const void* ptr, agg::int32u size);
        //---------------------------------------------------------------------
        template <class T>
        void add(const T& attr)
        {
            m_buffer.add_array(attr.head(), attr.head_size());
            m_buffer.add_array(attr.data(), attr.data_size());
        }
        //---------------------------------------------------------------------
    private:
        data_container m_buffer;
    };
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    inline void iomemstream::clear()
    {
        m_buffer.free_all();
    }
    //-------------------------------------------------------------------------
    inline agg::int32u iomemstream::size() const 
    { 
        return m_buffer.size();
    }
    //-------------------------------------------------------------------------
    inline data_container const & iomemstream::buffer() const 
    { 
        return m_buffer;
    }    
    //-------------------------------------------------------------------------
    inline data_container  & iomemstream::buffer()  
    { 
        return m_buffer;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::operator<<(agg::int8u v)
    {
        m_buffer.add(v);
        return *this;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::operator<<(agg::int32u v)
    {
        m_buffer.add_array((const agg::int8u *)&v, sizeof(agg::int32u));
        return *this;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::operator<<(const ptr_size & ps)
    {
        (*this) << ps.m_size;
        m_buffer.add_array(ps.m_ptr, ps.m_size);
        return *this;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::operator<<(const ptr_size_short& ps)
    {
        m_buffer.add(ps.m_size);
        m_buffer.add_array(ps.m_ptr, ps.m_size);
        return *this;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::operator<<(data_accessor_type data)
    {
        while (data.size())
        {
            m_buffer.add(*data);
            ++data;
        }        
        return *this;
    }
    //-------------------------------------------------------------------------
    inline iomemstream& iomemstream::write(const void * ptr, agg::int32u size)
    {
        assert(ptr);

        const agg::int8u * p = (const agg::int8u *)ptr;
        while (size--)
        {
            m_buffer.add(*p++);
        }

        return *this;
    }

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_IOMEMSTREAM_INCLUDE







